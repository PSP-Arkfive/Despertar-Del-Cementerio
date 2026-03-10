add_prx_module(idstorage exports.exp)

target_sources(idstorage PRIVATE main.c)
target_link_libraries(idstorage PRIVATE -nostdlib iplsdk ansic idstorage_ms pspmodinfo pspsdk pspkernel)