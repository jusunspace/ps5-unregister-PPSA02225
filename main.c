#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>

#define TITLE_ID "PPSA02225"
#define LOG_PATH "/data/unregister_PPSA02225.log"

int sceAppInstUtilInitialize(void);
int sceAppInstUtilTerminate(void);
int sceAppInstUtilAppUnInstall(const char *title_id);

static int exists(const char *p) {
    struct stat st;
    return stat(p, &st) == 0;
}

static void log_line(const char *name, int value) {
    FILE *f = fopen(LOG_PATH, "a");
    if (f) {
        fprintf(f, "%s: 0x%08X (%d)\n", name, (uint32_t)value, value);
        fclose(f);
    }
    printf("%s: 0x%08X (%d)\n", name, (uint32_t)value, value);
}

int main(void) {
    static const char *paths[] = {
        "/user/app/" TITLE_ID,
        "/system_ex/app/" TITLE_ID,
        "/mnt/ext0/user/app/" TITLE_ID,
        "/mnt/ext1/user/app/" TITLE_ID,
        NULL
    };

    FILE *f = fopen(LOG_PATH, "w");
    if (f) {
        fprintf(f, "PPSA02225 AppInst unregister diagnostic\n");
        fprintf(f, "Target: %s\n", TITLE_ID);
        fclose(f);
    }

    /* Safety guard: never uninstall while physical game files still exist. */
    for (int i = 0; paths[i]; ++i) {
        if (exists(paths[i])) {
            f = fopen(LOG_PATH, "a");
            if (f) {
                fprintf(f, "ABORT: physical install exists: %s\n", paths[i]);
                fclose(f);
            }
            return 2;
        }
    }

    int rc = sceAppInstUtilInitialize();
    log_line("sceAppInstUtilInitialize", rc);
    if (rc != 0)
        return 3;

    rc = sceAppInstUtilAppUnInstall(TITLE_ID);
    log_line("sceAppInstUtilAppUnInstall", rc);

    int trc = sceAppInstUtilTerminate();
    log_line("sceAppInstUtilTerminate", trc);

    return rc == 0 ? 0 : 4;
}
