#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#if __linux__
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#endif

#include "obs_shared.h"
#include "obs_plugin.h"

ObsStats* stats = NULL;

int main()
{
    char col1[9], col2[9];
    memset(col1, 0, sizeof(col1));
    memset(col2, 0, sizeof(col2));

#ifdef __linux__
    int fd;
    char shmpath[1024];
    mangohud_obs_get_shmpath(shmpath, sizeof(shmpath));
    if ((fd = shm_open(shmpath, O_CREAT | O_RDWR, 0666)) < 0)
    {
        fprintf(stderr,"shm_open %s\n", strerror(errno));
    }
    if(fd > 0 && ftruncate(fd, sizeof(ObsStats)) < 0)
    {
        fprintf(stderr, "ftruncate %s\n", strerror(errno));
    }
    if ((stats = mmap(NULL, sizeof(ObsStats), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "mmap %s\n", strerror(errno));
    }else {
        /*TODO: read MangoHud.conf ? */
        /*stats->prefix_exe = prefix_exe;*/
        stats->prefix_exe = 1;
        stats->running_mangohud = 1;
        FILE* procnameptr = fopen("/proc/self/comm", "rb");
        fgets(stats->exe, sizeof(stats->exe), procnameptr);
        fclose(procnameptr);
        /*printf("%s", stats->exe);*/
    }
#endif

    if(stats && stats->recording)
    {
        char mins_secs[6];
        time_t t = stats->time;
        struct tm* tm = gmtime(&t);
        strftime(mins_secs, sizeof(mins_secs), "%M:%S", tm);
        uint32_t hrs = t / 3600;
        snprintf(col1, sizeof(col1), "%u:%s", hrs, mins_secs);
        snprintf(col2, sizeof(col2), "%.1fMiB", stats->bytes / 1024.0 / 1024.0);
    }else if(stats && !stats->recording){
        const char* notrecordingstate = "Inactive";
        if(stats->running_obs)
            notrecordingstate = "Ready";
        snprintf(col1, sizeof(col1), "%s", notrecordingstate);
    }else{
        snprintf(col1, sizeof(col1), "%s", "Error");
    }
    printf("%s %s\n", col1, col2);
    return 0;
}
