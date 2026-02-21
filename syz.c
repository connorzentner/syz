#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <limits.h>
#include <pwd.h>

// Macros for text formating
#define TEXT_BOLD  "\033[1m"
#define TEXT_RESET "\033[0m"

// Data structures
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
} OSInfo;

typedef struct {
    char shell[64];
} Shell;

// Function for returning memory
Memory get_memory() {
    long total = 0, available = 0;
    FILE *fp = fopen("/proc/meminfo", "r");

    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "MemTotal: %ld", &total) == 1) continue;
            if (sscanf(line, "MemAvailable: %ld", &available) == 1) continue;
        }
        fclose(fp);
    }
    
    return (Memory){
        .total = total / 1024,
        .used = (total - available) / 1024
    };
}

// Function for returning uptime
Uptime get_uptime() {
    struct sysinfo info;
    
    // Initializing the struct to avoid garbage value
    if (sysinfo(&info) != 0) {
        return (Uptime){0, 0};
    }

    long s = info.uptime;
    int h = s / 3600;
    int m = (s % 3600) / 60;

    return (Uptime) {
        .hours = (int)(info.uptime / 3600),
        .minutes = (int)((info.uptime % 3600) / 60)
    };
}

// Function for returning host and user names
UserHost get_user_host() {
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

// Funtion for returning OS info
OSInfo get_os_info() {
    OSInfo os = {"Unknown"};
    FILE *fp = fopen("/etc/os-release", "r");

    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
                char *name_start = line + 12;

                if (*name_start == '\"') name_start++;

                char *name_end = strpbrk(name_start, "\"\n");
                if (name_end) *name_end = '\0';

                strncpy(os.name, name_start, sizeof(os.name) - 1);
                break;
            }
        }
        fclose(fp);
    }
    return os;
}

Shell get_shell() {
    Shell s = { .shell = "unknown" };
    pid_t ppid = getppid();
    char path[64];

    snprintf(path, sizeof(path), "/proc/%d/comm", ppid);

    FILE *fp = fopen(path, "r");
    if (fp) {
        if (fgets(s.shell, sizeof(s.shell), fp)) {
            // Removing trailing newline
            s.shell[strcspn(s.shell, "\n")] = 0;
        }
        fclose(fp);
    }
    return s;
}

// Driver function
int main() {
    struct utsname buffer;
    uname(&buffer);

    Memory mem = get_memory();
    Uptime up = get_uptime();
    UserHost uh = get_user_host();
    OSInfo os = get_os_info();
    Shell s = get_shell();

    printf("\n");
    printf(" (       )    )  \n");
    printf(" )\\ ) ( /( ( /(  \n");
    printf("(()/( )\\()))\\()) \n");
    printf(" /(_)|(_)\\((_)\\  \n");
    printf("(_))__ ((_)_((_) \n");
    printf("/ __\\ \\ / /_  /  \n");
    printf("\\__ \\\\ V / / /   \n");
    printf("|___/ |_| /___|  \n");

    printf("\n");
    printf(TEXT_BOLD "User:   " TEXT_RESET "%s\n", uh.user);
    printf(TEXT_BOLD "Host:   " TEXT_RESET "%s\n", uh.host);
    printf(TEXT_BOLD "OS:     " TEXT_RESET "%s\n", os.name);
    printf(TEXT_BOLD "Kernel: " TEXT_RESET "%s %s\n", buffer.sysname, buffer.release);
    printf(TEXT_BOLD "System: " TEXT_RESET "%s\n", buffer.machine);
    printf(TEXT_BOLD "Shell:  " TEXT_RESET "%s\n", s.shell);
    printf(TEXT_BOLD "Uptime: " TEXT_RESET "%dh %dm\n", up.hours, up.minutes);
    printf(TEXT_BOLD "Memory: " TEXT_RESET "%ld | %ld MB\n", mem.used, mem.total);
    printf("\n");

    return 0;
}
