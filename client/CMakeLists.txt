cmake_minimum_required(VERSION 2.6)

project(DynamoRIO_samples)

add_library(myclient SHARED myclient.c;myclient.h)   #多个.c文件用;隔开
find_package(DynamoRIO ${VERSION_NUMBER_MAJOR}.${VERSION_NUMBER_MINOR})
if (NOT DynamoRIO_FOUND)
    message(FATAL_ERROR "DynamoRIOpackage required to build")
endif(NOT DynamoRIO_FOUND)
configure_DynamoRIO_client(myclient)
##client调用API全部写出
use_DynamoRIO_extension(myclient drmgr)   
use_DynamoRIO_extension(myclient drwrap)
use_DynamoRIO_extension(myclient drreg)
use_DynamoRIO_extension(myclient drutil)
use_DynamoRIO_extension(myclient drx)
# use_DynamoRIO_extension(myclient drx)
