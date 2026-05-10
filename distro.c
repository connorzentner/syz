#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "distro.h"

void
print_distro_ascii(const char *id)
{
    bool show_distro = true;

    if (!show_distro)
        goto generic;

    if (show_distro) {
        if (strcmp(id, "arch") == 0) {
            printf("\n");
            printf(BOLD CYAN "   ┏┓   ┓ \n"
                             "   ┣┫┏┓┏┣┓\n"
                             "   ┛┗┛ ┗┛┗\n" RESET);
        } else if (strcmp(id, "debian") == 0) {
            printf("\n");
            printf(BOLD RED "   ┳┓  ┓ •    \n"
                            "   ┃┃┏┓┣┓┓┏┓┏┓\n"
                            "   ┻┛┗ ┗┛┗┗┻┛┗\n" RESET);
        } else if (strcmp(id, "nixos") == 0) {
            printf("\n");
            printf(BOLD BLUE "   ┳┓•  ┏┓┏┓\n"
                             "   ┃┃┓┓┏┃┃┗┓\n"
                             "   ┛┗┗┛┗┗┛┗┛\n" RESET);
        } else if (strcmp(id, "manjaro") == 0) {
            printf("\n");
            printf(BOLD GREEN "   ┳┳┓    •      \n"
                              "   ┃┃┃┏┓┏┓┓┏┓┏┓┏┓\n"
                              "   ┛ ┗┗┻┛┗┃┗┻┛ ┗┛\n"
                              "          ┛      \n" RESET);
        }
        else if (strcmp(id, "opensuse") == 0) {
            printf("\n");
            printf(BOLD GREEN "           ┏┓┳┳┏┓┏┓\n"
                              "   ┏┓┏┓┏┓┏┓┗┓┃┃┗┓┣ \n"
                              "   ┗┛┣┛┗ ┛┗┗┛┗┛┗┛┗┛\n"
                              "     ┛             \n" RESET);
        } else if (strcmp(id, "linuxmint") == 0) {
            printf("\n");
            printf(BOLD GREEN "   ┳┳┓•    \n"
                              "   ┃┃┃┓┏┓╋ \n"
                              "   ┛ ┗┗┛┗┗ \n" RESET);
        } else if (strcmp(id, "kali") == 0) {
            printf("\n");
            printf(BOLD BLUE "   ┓┏┓  ┓•\n"
                             "   ┃┫ ┏┓┃┓\n"
                             "   ┛┗┛┗┻┗┗\n" RESET);
        } else if (strcmp(id, "gentoo") == 0) {
            printf("\n");
            printf(BOLD MAGENTA  "   ┏┓         \n"
                                 "   ┃┓┏┓┏┓╋┏┓┏┓\n"
                                 "   ┗┛┗ ┛┗┗┗┛┗┛\n" RESET);
        } else if (strcmp(id, "fedora") == 0) {
            printf("\n");
            printf(BOLD BLUE "   ┏┓   ┓      \n"
                             "   ┣ ┏┓┏┫┏┓┏┓┏┓\n"
                             "   ┻ ┗ ┗┻┗┛┛ ┗┻\n" RESET);
        } else if (strcmp(id, "ubuntu") == 0) {
            printf("\n");
            printf(BOLD YELLOW  "   ┳┳┓        \n"
                                "   ┃┃┣┓┓┏┏┓╋┓┏\n"
                                "   ┗┛┗┛┗┻┛┗┗┗┻\n" RESET);
        } else {
        generic:
            printf("\n");
            printf(BOLD WHITE  "   ┏┓┓┏┏┓\n"
                        RED    "   ┗┓┗┫┏┛\n"
                        YELLOW "   ┗┛┗┛┗┛\n" RESET);
        }
    }
}
