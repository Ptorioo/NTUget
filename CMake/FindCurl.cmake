IF(CURL_INCLUDE_DIRS)
  SET(CURL_FIND_QUIETLY TRUE)
ENDIF (CURL_INCLUDE_DIRS)

FILE(GLOB_RECURSE POTENTIAL_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/curl)

FIND_PATH(CURL_INCLUDE_DIR
    NAMES curl.h
    PATHS
        ${POTENTIAL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        /usr/include/curl
        /usr/include/x86_64-linux-gnu/curl
        /usr/include/riscv64-linux-gnu/curl
        /usr/include/aarch64-linux-gnu/curl
    PATH_SUFFIXES curl
)

FIND_LIBRARY(CURL_LIBRARY
    NAMES curl
    PATHS
    /usr/lib/x86_64-linux-gnu/
    /usr/local/lib/
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(curl DEFAULT_MSG CURL_LIBRARY CURL_INCLUDE_DIR)

IF(CURL_FOUND)
  SET( CURL_LIBRARIES ${CURL_LIBRARY} )
  SET( CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIR} )
ELSE(CURL_FOUND)
  SET( CURL_LIBRARIES )
  SET( CURL_INCLUDE_DIRS )
ENDIF(CURL_FOUND)

MARK_AS_ADVANCED( CURL_LIBRARIES CURL_INCLUDE_DIRS )