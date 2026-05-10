#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <limits.h>
#include <pwd.h>

#include "distro.h"

#define BOX_WIDTH 24

typedef struct {
    long used;
    long total;
} Memory;

typedef struct {
    int minutes;
    int hours;
} Uptime;

typedef struct {
    char user[64];
    char host[64];
} UserHost;

typedef struct {
    char name[128];
    char id[32];
} OSInfo;

typedef struct {
    char shell[64];
} Shell;

static
Memory get_memory(void) {
    long total = 0, available = 0;
    FILE *fp = fopen("/proc/meminfo", "r");

    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp)) {

            if (sscanf(line, "MemTotal: %ld", &total) == 1)
                continue;

            if (sscanf(line, "MemAvailable: %ld", &available) == 1)
                continue;
        }
        fclose(fp);
    }

    return (Memory) {
        .total = total / 1024,
        .used = (total - available) / 1024
    };
}

static
Uptime get_uptime(void)
{
    struct sysinfo info;

    /* Initializing to avoid garbage value */
    if (sysinfo(&info) != 0)
        return (Uptime){0, 0};

    long s = info.uptime;
    int h = s / 3600;
    int m = (s % 3600) / 60;

    return (Uptime) {
        .hours = h,
        .minutes = m
    };
}

static
UserHost get_user_host(void)
{
    UserHost uh = {0};

    gethostname(uh.host, sizeof(uh.host));
    struct passwd *pw = getpwuid(geteuid());

    if (pw) {
        snprintf(uh.user, sizeof(uh.user), "%s", pw->pw_name);
    } else {
        snprintf(uh.user, sizeof(uh.user), "unknown");
    }

    return uh;
}

static
OSInfo get_os_info(void)
{
    OSInfo os = {0};
    strncpy(os.name, "generic", sizeof(os.name) - 1);
    FILE *fp = fopen("/etc/os-release", "r");

    if (fp) {
        char line[256];
        bool found_name = false;
        bool found_id = false;

        while (fgets(line, sizeof(line), fp)) {
            if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
                char *name_start = line + 12;
                if (*name_start == '\"')
                    name_start++;

                char *name_end = strpbrk(name_start, "\"\n");
                if (name_end)
                    *name_end = '\0';

                strncpy(os.name, name_start, sizeof(os.name) - 1);
                found_name = true;
            }

            else if (strncmp(line, "ID=", 3) == 0) {
                char *start = line + 3;
                if (*start == '\"') start++;
                char *end = strpbrk(start, "\"\n");
                if (end) *end = '\0';
                strncpy(os.id, start, sizeof(os.id) - 1);
                found_id = true;
            }
            if (found_name && found_id)
                break;
        }
        fclose(fp);
    }
    return os;
}

static
Shell get_shell(void)
{
    Shell sh = { .shell = "unknown" };
    pid_t ppid = getppid();
    char path[64];

    snprintf(path, sizeof(path), "/proc/%d/comm", ppid);

    FILE *fp = fopen(path, "r");
    if (fp) {
        if (fgets(sh.shell, sizeof(sh.shell), fp)) {
            sh.shell[strcspn(sh.shell, "\n")] = 0;
        }
        fclose(fp);
    }
    return sh;
}

static void
print_row(const char* color,
		  const char* value)
{
	printf(RESET " │ %s%-*s" RESET " │\n", color, BOX_WIDTH, value);
}

int
main(void)
{
    struct utsname buffer;
    uname(&buffer);

    Memory mem = get_memory();
    Uptime up = get_uptime();
    UserHost uh = get_user_host();
    OSInfo os = get_os_info();
    Shell sh = get_shell();

    char kernel_full[256];
    char uptime_full[64];
    char mem_full[256];

    snprintf(kernel_full, sizeof(kernel_full), "%s %s", buffer.sysname, buffer.release);
    snprintf(uptime_full, sizeof(uptime_full), "%dh %dm", up.hours, up.minutes);
    snprintf(mem_full, sizeof(mem_full), "%ld MiB / %ld MiB", mem.used, mem.total);

    print_distro_ascii(os.id);

	const char *top    = " ┌──────────────────────────┐";
    const char *mid    = " ├──────────────────────────┤";
    const char *bottom = " └──────────────────────────┘";

    printf(RESET "%s\n", top);
	print_row(RESET, uh.user);
    print_row(RESET, uh.host);
    print_row(RESET, sh.shell);
    print_row(RESET, os.name);
    print_row(RESET, kernel_full);
    print_row(RESET, uptime_full);
    print_row(RESET, mem_full);
    printf(RESET "%s\n", mid);

    printf(RESET " │ ");
    for (int i = 0; i < 8; i++) {
        printf("\033[4%dm   ", i);
    }
    printf(RESET " │\n");

    printf(RESET " │ ");
    for (int i = 0; i < 8; i++) {
        printf("\033[10%dm   ", i);
    }
    printf(RESET " │\n");

    printf(RESET "%s\n", bottom);
	printf("\n");

    return 0;
}
