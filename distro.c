#include <stdio.h>
#include <string.h>
#include "colors.h"

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
    else if (strcmp(id, "manjaro") == 0) {
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
    }
    else if (strcmp(id, "mint") == 0) {
        printf("\n");
        printf(BOLD GREEN "   ┳┳┓•    \n"
                          "   ┃┃┃┓┏┓╋ \n"
                          "   ┛ ┗┗┛┗┗ \n" RESET);
    }
    else if (strcmp(id, "kali") == 0) {
        printf("\n");
        printf(BOLD BLUE "   ┓┏┓  ┓•\n"
                         "   ┃┫ ┏┓┃┓\n"
                         "   ┛┗┛┗┻┗┗\n" RESET);
    }
    else if (strcmp(id, "gentoo") == 0) {
        printf("\n");
        printf(BOLD MAGENTA  "   ┏┓         \n"
                             "   ┃┓┏┓┏┓╋┏┓┏┓\n"
                             "   ┗┛┗ ┛┗┗┗┛┗┛\n" RESET);
    }
    else if (strcmp(id, "fedora") == 0) {
        printf("\n");
        printf(BOLD BLUE "   ┏┓   ┓      \n"
                         "   ┣ ┏┓┏┫┏┓┏┓┏┓\n"
                         "   ┻ ┗ ┗┻┗┛┛ ┗┻\n" RESET);
    } 
    else if (strcmp(id, "ubuntu") == 0) {
        printf("\n");
        printf(BOLD YELLOW  "   ┳┳┓        \n"
                            "   ┃┃┣┓┓┏┏┓╋┓┏\n"
                            "   ┗┛┗┛┗┻┛┗┗┗┻\n" RESET);
    }
    else {
        printf(BOLD WHITE  "   ┏┓┓┏┏┓\n"
                    RED    "   ┗┓┗┫┏┛\n"
                    YELLOW "   ┗┛┗┛┗┛\n" RESET);
            }
}
