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
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BLACK   "\033[30m"

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
    char id[32];
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
    OSInfo os = {"generic"};
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

            if (strncmp(line, "ID=", 3) == 0) {
                char *start = line + 3;
                if (*start == '\"') start++;
                char *end = strpbrk(start, "\"\n");
                if (end) *end = '\0';
                strncpy(os.id, start, sizeof(os.id) - 1);
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

// Function for printing distro
void print_ascii_art(const char *id) {
    if (strcmp(id, "arch") == 0) {
        printf("\n");
        printf(BOLD CYAN "   ┏┓   ┓ \n"
                         "   ┣┫┏┓┏┣┓\n"
                         "   ┛┗┛ ┗┛┗\n" RESET);
    }
    else if (strcmp(id, "debian") == 0) {
        printf("\n");
        printf(BOLD RED "   ┳┓  ┓ •    \n"
                        "   ┃┃┏┓┣┓┓┏┓┏┓\n"
                        "   ┻┛┗ ┗┛┗┗┻┛┗\n" RESET);
    }
    else if (strcmp(id, "nixos") == 0) {
        printf("\n");
        printf(BOLD BLUE "   ┳┓•  ┏┓┏┓\n"
                         "   ┃┃┓┓┏┃┃┗┓\n"
                         "   ┛┗┗┛┗┗┛┗┛\n" RESET);
    }
    else {
        printf(BOLD WHITE  "   ┏┓┓┏┏┓\n"
                    RED    "   ┗┓┗┫┏┛\n"
                    YELLOW "   ┗┛┗┛┗┛\n" RESET);
            }
}

// Function for border formating
void print_row(const char* icon, const char* label, const char* color, const char* value) {
    printf(WHITE "  │ " RESET "%s%s " WHITE "%-6s " WHITE "│ " RESET "%s%s\n", 
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

    char kernel_full[64], uptime_full[32], mem_full[32];
    snprintf(kernel_full, sizeof(kernel_full), "%s %s", buffer.sysname, buffer.release);
    snprintf(uptime_full, sizeof(uptime_full), "%dh %dm", up.hours, up.minutes);
    snprintf(mem_full, sizeof(mem_full), YELLOW "%ld" WHITE " | " BLUE "%ld", mem.used, mem.total);
    
    print_ascii_art(os.id);

    printf(WHITE "  ┌──────────┐\n");
    print_row(BOLD   "󰀆", "USER",    RED,     uh.user);
    print_row(BOLD   "󰇥", "HOST",    YELLOW,  uh.host);
    print_row(BOLD   "", "SHELL",   GREEN,   sh.shell);
    print_row(BOLD   "", "DISTRO",  CYAN,    os.name);
    print_row(BOLD   "󰌽", "KERNEL",  BLUE,    kernel_full);
    print_row(BOLD   "󰥔", "UPTIME",  MAGENTA, uptime_full);
    print_row(BOLD   "󰍛", "MEMORY",  WHITE,   mem_full);
    printf(WHITE "  ├──────────┤\n");
    printf(WHITE "  │ " RESET BOLD "󰏘 COLORS " WHITE "│ " RESET);
    int color_order[] = { 7, 1, 3, 2, 6, 4, 5, 0 };
    for (int i = 0; i < 8; i++) {
        printf("\033[3%dm󱡔 " RESET, color_order[i]);
    }
    printf("\n");
    printf(WHITE "  └──────────┘" RESET "\n\n");

    return 0;
}
