#ifndef __FLASHEMU_H__
#define __FLASHEMU_H__

struct SceIoDeviceTable;

/** Structure passed to the init and exit functions of the io driver system */
typedef struct SceIoDeviceEntry
{
	/** Pointer to the original driver which was added */
	struct SceIoDeviceTable *d_dp;
	/** Pointer to a user defined argument (if written by the driver will preseve across calls */
	void                    *d_private;
	int                      d_userfd_count;
} SceIoDeviceEntry;

typedef struct SceIoCwd
{
    struct SceIoCwd         *next;
    char                    *pathname;
    struct SceIoDeviceEntry *de;
    void                    *cwd_private;
    int                      refcount;
} SceIoCwd;

/** Structure passed to the file functions of the io driver system */
typedef struct SceIoIob
{
	/** Unknown */
        int               i_flgs;
	/** The file system number, e.g. if a file is opened as host5:/myfile.txt this field will be 5 */
	int               i_unit;
	/** Pointer to the driver structure */
	SceIoDeviceEntry *i_de;
	/** Unknown, again */
	int               d_type;
	/** Pointer to a user defined argument, this is preserved on a per file basis */
	void             *i_private;
	SceIoCwd         *i_cwd;
	SceOff            i_fpos;
	SceUID            i_thread;
	int               dummy;
} SceIoIob;

/** Structure to maintain the file driver pointers */
typedef struct SceIoDeviceFunction
{
	int      (* df_init)(SceIoDeviceEntry *);
	int      (* df_exit)(SceIoDeviceEntry *);
	int      (* df_open)(SceIoIob *, char *, int, SceMode);
	int      (* df_close)(SceIoIob *);
	SceSSize (* df_read)(SceIoIob *, void *, SceSize);
	SceSSize (* df_write)(SceIoIob *, void *, SceSize);
	SceOff   (* df_lseek)(SceIoIob *, SceOff, int);
	int      (* df_ioctl)(SceIoIob *, unsigned int, void *, SceSize, void *, SceSize);
	int      (* df_remove)(SceIoIob *, const char *);
	int      (* df_mkdir)(SceIoIob *, const char *, SceMode);
	int      (* df_rmdir)(SceIoIob *, const char *);
	int      (* df_dopen)(SceIoIob *, const char *);
	int      (* df_dclose)(SceIoIob *);
	int      (* df_dread)(SceIoIob *, SceIoDirent *);
	int      (* df_getstat)(SceIoIob *, const char *, SceIoStat *);
	int      (* df_chstat)(SceIoIob *, const char *, SceIoStat *, int);
	int      (* df_rename)(SceIoIob *, const char *, const char *);
	int      (* df_chdir)(SceIoIob *, const char *);
	int      (* df_mount)(SceIoIob *);
	int      (* df_umount)(SceIoIob *);
	int      (* df_devctl)(SceIoIob *, const char *, unsigned int, void *, SceSize, void *, SceSize);
	int      (* df_cancel)(SceIoIob *);
} SceIoDeviceFunction;

typedef struct SceIoDeviceTable
{
	/** The name of the device to add */
	const char          *dt_string;
	/** Device type, this 0x10 is for a filesystem driver */
	int                  dt_type;
	/** Unknown, set to 0x800 */
	int                  dt_size;
	/** This seems to be the same as name but capitalised :/ */
	const char          *dt_desc;
	/** Pointer to a filled out functions table */
	SceIoDeviceFunction *dt_func;
} SceIoDeviceTable;

int InstallFlashEmu();
int UninstallFlashEmu();
void PrepareRebootFlashEmu();

#endif