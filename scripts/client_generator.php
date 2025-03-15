<?php

class OpenBLASClientGenerator
{
    public array $excludeFuncs = [
        'openblas_setaffinity',
        'openblas_getaffinity',
        'cblas_csrot',
        'cblas_zdrot',
        'cblas_crotg',
        'cblas_zrotg',
        'cblas_xerbla',
        'cblas_sbstobf16',
        'cblas_sbdtobf16',
        'cblas_sbf16tos',
        'cblas_dbf16tod',
        'cblas_sbdot',
        'cblas_sbgemv',
        'cblas_sbgemm',
    ];

    public array $funcDecrWord = [
        'int','char*','void','float','double','CBLAS_INDEX',
        //'openblas_complex_float','openblas_complex_double',
        'lapack_int',
        //'lapack_complex_float','lapack_complex_double',
        'int32_t','int64_t','void*',
    ];

    public array $funcPrefixWord = [
        'RINDOW_FUNC_DECL',
    ];

    public string $preg_types;
    public string $preg_prefix;
    private string $destDir;

    public function __construct()
    {
        $this->preg_types = implode('|',array_map(fn($x) => str_replace('*','\\*',$x),$this->funcDecrWord));
        $this->preg_prefix = implode('|',$this->funcPrefixWord);
    }

    private function writeFunc(string $funcName,string $code) : void
    {
        $fileName = $this->destDir.'/'.$funcName.'.c';
        if(file_put_contents($fileName,$code)===false) {
            throw new RuntimeException("write error: ".$fileName);
        }
    }

    public function generator($argv) : void
    {
        if(count($argv)==1) {
            echo "Usage: php client_generator.php dest-dir [cblas|lapacke|matlib] inputheaders...\n";
            return;
        }
        $dummy = array_shift($argv);
        $this->destDir = array_shift($argv);
        $lastpath = substr($this->destDir,-1,1);
        if(in_array($lastpath,['/','\\'])) {
            $this->destDir = substr($this->destDir,0,-1);
        }
        if(!is_dir($this->destDir)) {
            throw new InvalidArgumentException('destination directory not found.: '.$this->destDir);
        }
        $headertype = array_shift($argv);
        $fpi = null;
        $fpo = null;
        $funcs = [];
        try {
            $code = $this->beginTemplate($headertype);
            $this->writeFunc('common',$code);
            while($inputfile=array_shift($argv)) {
                $fpi = fopen($inputfile,'r');
                if($fpi==null) {
                    throw new RuntimeException("Error opening input file: $inputfile");
                }
                $funcs = array_merge($funcs,$this->generate($fpi,$headertype));
            }
        } finally {
            if($fpi) {
                fclose($fpi);
            }
        }
    }
    
    public function generate(mixed $fpi,string $headertype) : array
    {
        $excludeFuncs = $this->excludeFuncs;
        $eof = false;
        $funcs = [];
        while(!$eof) {
            $line='';
            while(true) {
                $next=fgets($fpi);
                if(!$next) {
                    $eof = true;
                    break;
                }
                $next = trim($next);
                if(substr($next,0,1)=='#') {
                    break;
                }
                if(substr($next,0,2)=='/*') {
                    break;
                }
                if(substr($next,0,2)=='//') {
                    break;
                }
                $line .= trim($next);
                if(substr($next,-1,1)==';') {
                    break;
                }
            }
            $declare = $this->parser($line);
            //var_dump($line);
            //var_dump($declare);
            //echo "PAUSE>";
            //fgets(STDIN);
            if($declare==null) {
                continue;
            }
            if(in_array($declare['func'],$excludeFuncs)) {
                continue;
            }
            $code = $this->funcTemplate($declare,$headertype);
            $this->writeFunc($declare['func'],$code);
            $funcs[] = $declare;
        }
        return $funcs;
    }
    
    public function parser(string $line) : ?array
    {
        $tmp = explode(' ',$line);
        $head = $tmp[0];
        if(in_array($head,$this->funcDecrWord)) {
            $pattern = "/^(".$this->preg_types.") *([A-Za-z0-9_]+) *\\(([^)]+)/";
            preg_match($pattern,$line,$match);
        } elseif(in_array($head,$this->funcPrefixWord)) {
            $pattern = "/^(".$this->preg_prefix.")* *(".$this->preg_types.") *([A-Za-z0-9_]+) *\\(([^)]+)/";
            preg_match($pattern,$line,$match);
            $prefix = array_shift($match);
        } else {
            return null;
        }
        if(array_key_exists(1,$match)) {
            $return = $match[1];
        } else {
            var_dump("unmatch return $line");
            $return = "unknown";
        }
        if(array_key_exists(2,$match)) {
            $func = $match[2];
        } else {
            var_dump("unmatch func $line");
            $func = "unknown";
        }
        if(array_key_exists(3,$match)) {
            $args = $match[3];
        } else {
            var_dump("unmatch args $line");
            $args = ["unknown","unknown"];
        }
        $args = explode(',',$args);
        $strargs = array_map('trim',$args);
        $args = [];
        foreach($strargs as $arg) {
            $var = null;
            $arg = str_replace('*',' *',$arg);
            $types2 = explode(' ',$arg);
            $types = [];
            foreach($types2 as $t) {
                if($t) {
                    $types[] = $t;
                }
            }
            $types = array_map('trim',$types);
            if(count($types)>1) {
                $var = array_pop($types);
                $var = trim($var);
                if(substr($var,0,1)=='*') {
                    $var = substr($var,1);
                    array_push($types,'*');
                }
            }
            $type = implode(' ',$types);
            $args[] = ['type'=>$type,'var'=>$var];
        }
        //var_dump("$return $func");
        //echo "args:";
        //var_dump($args);
        //if($func=='cblas_xerbla') {
        //    var_dump(['return' => $return, 'func'=>$func, 'args'=>$args]);
        //}
        
        return ['return' => $return, 'func'=>$func, 'args'=>$args];
    }

    private function headersTemplate(string $type) : string
    {
        $header = "#include <Windows.h>\n";
        switch ($type) {
            case 'cblas':
                $header .= "#include <cblas.h>\n";
                $header .= "#include \"vclib.h\"\n";
                break;
            case 'lapacke':
                $header .= "#if _MSC_VER\n";
                $header .= "#include <complex.h>\n";
                $header .= "#define lapack_complex_float _Fcomplex\n";
                $header .= "#define lapack_complex_double _Dcomplex\n";
                $header .= "#endif\n";
                $header .= "#include <lapacke.h>\n";
                $header .= "#include \"vclib.h\"\n";
                break;
            case 'matlib':
                $header .= "#include <rindow/matlib.h>\n";
                $header .= "#include \"vclib.h\"\n";
                break;
            default:
                throw new \Exception("unkown header type");
                break;
        }
        return $header;
    }

    private function libname($type) : string
    {
        switch($type) {
            case 'cblas':
            case 'lapacke':
                $libname = 'libopenblas';
                break;
            case 'matlib':
                $libname = 'rindowmatlib';
                break;
            default:
                throw new \Exception("unkown header type");
                break;
        }
        return $libname;
    }
    public function funcTemplate(array $declare,string $type) : string
    {
        $return = $declare['return'];
        $funcname = $declare['func'];
        $libname = $this->libname($type);
        // typedef 
        $code  = $this->headersTemplate($type);
        $code .= "static char msg_function_not_found[]  = \"{$funcname} not found\\n\";\n";
        $code .= "typedef {$return} (CALLBACK* PFN{$funcname})( /* {$funcname} */";
        $isNext = false;
        foreach($declare['args'] as $arg) {
            $type = $arg['type'];
            $var = $arg['var'];
            if($isNext) {
                $code .= ",";
            }
            $code .= "\n";
            $code .= "    {$type}            /* {$var} */";
            $isNext = true;
        }
        $code .= "\n";
        $code .= ");\n";
    
        // static function pointer
        $code .= "static PFN{$funcname} _g_{$funcname} = NULL;\n";
    
        // proxy function
        $code .= "{$return} {$funcname}(";
        $isNext = false;
        foreach($declare['args'] as $arg) {
            $type = $arg['type'];
            $var = $arg['var'];
            if($isNext) {
                $code .= ",";
            }
            $code .= "\n";
            $code .= "    {$type}            {$var}";
            $isNext = true;
        }
        $code .= "\n";
        $code .= ")\n";
        $code .= "{\n";
        $code .= "    if(_g_{$funcname}==NULL) {\n";
        $code .= "        _g_{$funcname} = rindow_load_{$libname}_func(\"$funcname\"); \n";
        $code .= "        if(_g_{$funcname}==NULL) {\n";
        $code .= "            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);\n";
        $code .= "            WriteConsole(hStdOut, msg_function_not_found, sizeof(msg_function_not_found), NULL, NULL);\n";
        if($return=='void') {
            $code .= "            return;\n";
        } else {
            $code .= "            return 0;\n";
        }
        $code .= "        }\n";
        $code .= "    }\n";
        if($return=='void') {
            $code .= "    _g_{$funcname}(";
        } else {
            $code .= "    return _g_{$funcname}(";
        }
        $isNext = false;
        foreach($declare['args'] as $arg) {
            $type = $arg['type'];
            $var = $arg['var'];
            if($isNext) {
                $code .= ",";
            }
            $code .= "\n";
            if($var) {
                $code .= "        {$var}";
            }
            $isNext = true;
        }
        $code .= "    \n";
        $code .= "    );\n";
        $code .= "}\n";
        return $code;
    }
    
    function beginTemplate(string $type) : string
    {
        $libname = $this->libname($type);
        $code  = $this->headersTemplate($type);
        $code .= "\n";
        $code .= "static HMODULE _h_{$libname} = NULL;\n";
        $code .= "static char msg_load_error[]  = \"Load error: {$libname}\\n\";\n";
        $code .= "void* rindow_load_{$libname}_func(char *funcname)\n";
        $code .= "{\n";
        $code .= "    if(_h_{$libname}==NULL) {\n";
        $code .= "        _h_{$libname} = LoadLibraryA( \"{$libname}.dll\" );\n";
        $code .= "        if(_h_{$libname}==NULL) {\n";
        $code .= "            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);\n";
        $code .= "            WriteConsole(hStdOut, msg_load_error, sizeof(msg_load_error), NULL, NULL);\n";
        $code .= "            return NULL;\n";
        $code .= "        }\n";
        $code .= "    }\n";
        $code .= "    return GetProcAddress( _h_{$libname}, funcname );\n";
        $code .= "}\n";
        return $code;
    }
}

$generator = new OpenBLASClientGenerator();
$generator->generator($argv);
