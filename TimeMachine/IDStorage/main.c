#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <pspsdk.h>
#include <pspsysmem_kernel.h>
#include <pspiofilemgr.h>
#include <pspsysevent.h>

#include <systemctrl_ark.h>
#include <idstorage_ms.h>
#include <fat.h>

PSP_MODULE_INFO("sceIdStorage_Service", PSP_MODULE_KERNEL | PSP_MODULE_SINGLE_START | PSP_MODULE_SINGLE_LOAD | PSP_MODULE_NO_STOP, 1, 1);

int g_idst_sema = 0;
int g_curr_thid = -1;
SceUID g_idstorage_data_id = 0;
u8 *g_idstorage_data = NULL;
key_entry *key_entries = NULL;

int sceIdStorageInit(void)
{
    char idstor_path[0x40];

    g_idst_sema = sceKernelCreateSema("SceIdStorage", 1, 1, 1, NULL);

    if (g_idst_sema < 0)
        return g_idst_sema;

    if (MsFatMount() < 0)
        goto error_exit;

    _sceIdStorageGetMsFilePath(idstor_path, 1);

    if (MsFatOpen(idstor_path) < 0)
        goto error_exit;

    int file_sz = MsFatSeek(0, SEEK_END);

    if (file_sz < 0)
        goto error_exit;

    MsFatSeek(0, SEEK_SET);

    g_idstorage_data_id = sceKernelAllocPartitionMemory(1, "IdStorageLeafs", PSP_SMEM_High, file_sz, NULL);

    if (g_idstorage_data_id < 0)
        goto error_exit;

    g_idstorage_data = (u8 *)sceKernelGetBlockHeadAddr(g_idstorage_data_id);
    key_entries = (key_entry *)g_idstorage_data;

    u32 bytes_read;
    if ((bytes_read = MsFatRead(g_idstorage_data, file_sz)) < 0)
        goto error_exit;

    return 0;

error_exit:
    if (g_idst_sema)
        sceKernelDeleteSema(g_idst_sema);

    if (g_idstorage_data_id)
        sceKernelFreePartitionMemory(g_idstorage_data_id);

    return -1;
}

int sceIdStorageIsDirty(void)
{
    return 0;
}

int sceIdStorageFormat(void)
{
    return SCE_ERROR_NOT_SUPPORTED;
}

int sceIdStorageUnformat(void)
{
    return SCE_ERROR_NOT_SUPPORTED;
}

int sceIdStorageWriteLeaf(u16 key, void *buf)
{
    return SCE_ERROR_NOT_SUPPORTED;
}

int sceIdStorageFlush(void)
{
    return 0;
}

int sceIdStorageGetFreeLeaves(void)
{
    return SCE_ERROR_NOT_SUPPORTED;
}

int sceIdStorageEnumId(int cb, void *opt)
{
    return SCE_ERROR_NOT_SUPPORTED;
}

int sceIdStorageCreateLeaf(u16 id)
{
    return SCE_ERROR_NOT_SUPPORTED;
}

int sceIdStorageCreateAtomicLeaves(u16 *ids, s32 size)
{
    return SCE_ERROR_NOT_SUPPORTED;
}

int sceIdStorageDeleteLeaf(u16 id)
{
    return SCE_ERROR_NOT_SUPPORTED;
}

int sceIdStorageUpdate(short id, u32 offset, void *buf, u32 len)
{
    return SCE_ERROR_NOT_SUPPORTED;
}

int module_start(SceSize args, void *argp) __attribute__((alias("sceIdStorageModuleStart")));
int sceIdStorageModuleStart(SceSize args, void *argp)
{
    return sceIdStorageInit();
}

int module_reboot_before(SceSize args, void *argp) __attribute__((alias("sceIdStorageModuleRebootBefore")));
int sceIdStorageModuleRebootBefore(SceSize args, void *argp)
{
    sceKernelDeleteSema(g_idst_sema);
    sceKernelFreePartitionMemory(g_idstorage_data_id);
    return 0;
}

int sceIdStorageSuspend(void)
{
    return 0;
}

int sceIdStorageResume(void)
{
    return 0;
}

int sceIdStorageGetLeafSize(void)
{
    return LEAF_SIZE;
}

int sceIdStorageIsFormatted(void)
{
    return 1;
}

int sceIdStorageIsReadOnly(void)
{
    return 1;
}

int _sceIdStorageLock(void)
{
    int thid;
    int res;

    if ((thid = sceKernelGetThreadId()) < 0)
        return thid;

    if (g_curr_thid == thid)
        return 0;

    if ((res = sceKernelWaitSema(g_idst_sema, 1, 0)) < 0)
        return res;

    g_curr_thid = thid;

    return 0;
}

int _sceIdStorageUnlock(void)
{
    g_curr_thid = -1;
    sceKernelSignalSema(g_idst_sema, 1);

    return 0;
}

int read_ms_leaf(void *buf, leaf_entry *leaf)
{
    memcpy(buf, &g_idstorage_data[leaf->offset], leaf->len);

    return leaf->len;
}