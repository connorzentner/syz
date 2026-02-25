#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <limits.h>
#include <pwd.h>

// Macros for text formating
#define BOLD  "\033[1m"
#define RESET "\033[0m"
#define WHITE   "\033[37m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define BOX_COLOR "\033[38;5;239m"

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

// Function for returning shell
Shell get_shell() {
    Shell sh = { .shell = "unknown" };
    pid_t ppid = getppid();
    char path[64];

    snprintf(path, sizeof(path), "/proc/%d/comm", ppid);

    FILE *fp = fopen(path, "r");
    if (fp) {
        if (fgets(sh.shell, sizeof(sh.shell), fp)) {
            // Removing trailing newline
            sh.shell[strcspn(sh.shell, "\n")] = 0;
        }
        fclose(fp);
    }
    return sh;
}


void print_row(const char* icon, const char* label, const char* color, const char* value) {
    printf(BOX_COLOR "  │ " RESET "%s%s %-10s " BOX_COLOR "│  " RESET "%s%s\n", 
           color, icon, label, color, value);
}

// Driver function
int main() {
    struct utsname buffer;
    uname(&buffer);

    Memory mem = get_memory();
    Uptime up = get_uptime();
    UserHost uh = get_user_host();
    OSInfo os = get_os_info();
    Shell sh = get_shell();

    const char *red         = "\033[31m";
    const char *yellow      = "\033[33m";
    const char *reset       = "\033[0m";

    char kernel_full[64], uptime_full[32], mem_full[32];
    snprintf(kernel_full, sizeof(kernel_full), "%s %s", buffer.sysname, buffer.release);
    snprintf(uptime_full, sizeof(uptime_full), "%dh %dm", up.hours, up.minutes);
    snprintf(mem_full, sizeof(mem_full), "%ld/%ld MB", mem.used, mem.total);
    
    printf("\n");
    printf(BOLD "  %s (       )    )  \n", red);
    printf(BOLD "  %s )\\ ) ( /( ( /(  \n", red);
    printf(BOLD "  %s(()/( )\\()))\\()) \n", red);
    printf(BOLD "  %s /(_)|(_)\\((_)\\  \n", red);
    printf(BOLD "  %s(_))__ ((_)_((_) \n", red);
    printf(BOLD "  %s/ __\\ \\ / /_  /  \n", yellow);
    printf(BOLD "  %s\\__ \\\\ V / / /   \n", yellow);
    printf(BOLD "  %s|___/ |_| /___|  %s\n", yellow, reset);

    printf(BOX_COLOR "  ┌──────────────┐\n");

    print_row(BOLD "", "USER",    RED,     uh.user);
    print_row(BOLD "󰇥", "HOST",    YELLOW,  uh.host);
    print_row(BOLD "", "SHELL",   GREEN,   sh.shell);
    print_row(BOLD "", "DISTRO",  CYAN,    os.name);
    print_row(BOLD "󰌽", "KERNEL",  BLUE,    kernel_full);
    print_row(BOLD "󰥔", "UPTIME",  MAGENTA, uptime_full);
    print_row(BOLD "󰍛", "MEMORY",  WHITE,   mem_full);

    printf(BOX_COLOR "  ├──────────────┤\n");

    printf(BOX_COLOR "  │ " RESET BOLD "󰏘 COLORS     " BOX_COLOR "│  " RESET);
    
    int color_order[] = { 7, 1, 3, 2, 6, 4, 5 };
    for (int i = 0; i < 7; i++) {
        printf("\033[3%dm󰈸 " RESET, color_order[i]);
    }
    printf("\n");

    printf(BOX_COLOR "  └──────────────┘" RESET "\n\n");

    return 0;
}
