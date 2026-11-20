#include <string.h>
#include <pspkernel.h>
#include <pspiofilemgr.h>
#include <psppower.h>

#include "tm_package.h"

static const char *tm_files[] = {
    "/TM/tmctrl.prx",
    "/TM/DCARK/reboot.prx",
    "/TM/DCARK/tmhlpr.prx"
};

int ExtractTimeMachineFiles(const char *archive, int install)
{
    unsigned char buffer[16 * 1024];
    SceOff offsets[3];
    int sizes[3], count, result = -1;
    unsigned int found = 0;
    SceUID fd = sceIoOpen(archive, PSP_O_RDONLY, 0);
    if (fd < 0)
        return -1;

    SceOff total = sceIoLseek(fd, 0, PSP_SEEK_END);
    if (total < 4 || sceIoLseek(fd, 0, PSP_SEEK_SET) != 0 ||
        sceIoRead(fd, &count, sizeof(count)) != sizeof(count) ||
        count < 0 || count > (total - 4) / 5)
        goto done;

    for (int i = 0; i < count; i++) {
        int size;
        unsigned char length;
        char name[256];
        if (sceIoRead(fd, &size, sizeof(size)) != sizeof(size) || size < 0 ||
            sceIoRead(fd, &length, sizeof(length)) != sizeof(length) || !length ||
            sceIoRead(fd, name, length) != length || memchr(name, '\0', length))
            goto done;
        name[length] = '\0';
        SceOff position = sceIoLseek(fd, 0, PSP_SEEK_CUR);
        if (position < 0 || position > total || size > total - position)
            goto done;
        for (int j = 0; j < 3; j++) {
            if (strcmp(name, tm_files[j]) == 0) {
                if (!size || (found & (1U << j)))
                    goto done;
                offsets[j] = position;
                sizes[j] = size;
                found |= 1U << j;
                break;
            }
        }
        if (sceIoLseek(fd, size, PSP_SEEK_CUR) != position + size)
            goto done;
    }

    if (found != 7)
        goto done;
    if (install) {
        for (int i = 0; i < 3; i++) {
            char path[64] = "ms0:";
            strcat(path, tm_files[i]);
            if (sceIoLseek(fd, offsets[i], PSP_SEEK_SET) != offsets[i])
                goto done;
            SceUID output = sceIoOpen(path, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
            if (output < 0)
                goto done;
            int remaining = sizes[i];
            while (remaining > 0) {
                int amount = remaining < (int)sizeof(buffer) ? remaining : (int)sizeof(buffer);
                int read = sceIoRead(fd, buffer, amount);
                if (read <= 0 || sceIoWrite(output, buffer, read) != read)
                    break;
                remaining -= read;
                scePowerTick(0);
            }
            int close_result = sceIoClose(output);
            if (remaining || close_result < 0)
                goto done;
        }
    }
    result = 0;
done:
    if (sceIoClose(fd) < 0)
        result = -1;
    return result;
}
