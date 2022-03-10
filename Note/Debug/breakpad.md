更新下breakpad的使用：

\1. 编译安装breakpad

\2. 在发生crash的程序中加入下面代码：

```
#include "client/linux/handler/exception_handler.h" 

bool cb(const google_breakpad::MinidumpDescriptor & descriptor, 
        void * context, bool succeeded) {
  std::cout << descriptor.path() << std::endl;
  return true;
}

  google_breakpad::MinidumpDescriptor mdd(".");
  google_breakpad::ExceptionHandler eh(mdd, 0, cb, 0, true, -1);
```



\3. 编译出可执行文件'exec'后执行，crash后产生'ddmmpp.dmp'文件

\4. dump_syms exec > exec.sym, dump_syms为安装breakpad后的可执行文件

\5. head -n1 exec.sym, 得到一串16进制数'hex'

\6. mkdir -p symbols/exec/'hex'/; mv exec.sym symbols/exec/'hex'/

\7. minidump_stackwalk 'ddmmpp.dmp' symbols; 生成call stack, minidump_stackwalk为安装breakpad后的可执行文件

要看到crash发生时的行号，第三步编译时需加'-g';

同时，如果第三步编译时没有'-g', 而使用带'-g'的exec生成exec.sym，代码量大时call stack虽然有行号，但是错乱的。