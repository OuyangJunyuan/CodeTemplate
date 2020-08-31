# Glog_template

## CMakeList

```
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake)
find_package(Glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIRS})	#${GLOG_LIBRARIES}

```

或者下载Glog的 FindGlog.Cmake 然后复制到工程下的Module文件夹。

```cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/module)
```

## template

```cpp

#include <glog/logging.h>
#include <iostream>

int main(int argc,char **argv)
{
    google::InitGoogleLogging(argv[0]); // Initialize Google's logging library.
    //google::InstallFailureWriter(&FatalMessageDump); 配合使用，可以在程序出现严重错误时将详细的错误信息打印出来
    google::SetLogFilenameExtension("log_");// setting output  prefix-filename
    //level:INFO, WARNING, ERROR, and FATAL. FATAL will print and stop/kill the running program
    //high level will output to low level files
    google::SetLogDestination(google::INFO, "../log/info/");
    google::SetLogDestination(google::WARNING, "../log/warning/");
    google::SetLogDestination(google::GLOG_ERROR, "../log/error/");
    google::SetStderrLogging(google::WARNING); //level above argument will output on screen/terminal

    //FLAGS_logtostderr =true; //output on screen/terminal instead of log files

    //把LOG换成DLOG： output to log only when debug. no work on release mode
    LOG(INFO) << "Found " <<  0;
    LOG(WARNING) << "Found " << 1;
    //LOG_IF 条件输出log
    for (int i = 0; i <= 5; ++i) {
        LOG_IF(INFO, i > 3) <<"当前"<<i << "大于3";
    }
    //LOG_EVERY_N 每N次输出1次log
    for (int i = 0; i < 15; ++i) {
        LOG_EVERY_N(INFO, 5) << "第" << google::COUNTER << "次命中";
    }
    //LOG_EVERY_N 当条件满足时候每N次输出1次log
    for (int i = 0; i < 15; ++i) {
        LOG_IF_EVERY_N(INFO,(i>5) ,5) << "条件满足且第 " << google::COUNTER << "次击中";
    }
    //LOG_EVERY_N 当条件满足时候每N次输出1次log
    for (int i = 0; i < 15; ++i) {
        LOG_FIRST_N(INFO,5) << "前" << google::COUNTER << "次命中";
    }
    google::ShutdownGoogleLogging();
    return 0;
}



```





