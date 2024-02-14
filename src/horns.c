/*
   Application to notify with (SMS) when a docker container
    closed .

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include "twilio.h"

int is_c_running(const char* c_name){
    char command[100];
    sprintf(command, "docker inspect --format={{.State.Running}} %s", c_name);

    FILE* pipe = popen(command, "r");
    if (!pipe) {
        perror("popen");
        return -1;
    }

    char result[10];
    if (fgets(result, sizeof(result), pipe) == NULL) {
        perror("fgets");
        pclose(pipe);
        return -1;
    }

    pclose(pipe);

    if (strcmp(result, "true\n") == 0) {
        return 1;
    } else {
        return 0;
    }
    
}


int main(int argc, char *argv[])
{
        int cmd;
        bool verbose = false;
        char *account_sid = "ACd36b85f3ba5905a85adb5d7bb78aaf4e";
        char *auth_token = "c34b7f2a958f5168abe4b704b7570bdd";
        char *message = NULL;
        char *from_number = "+13258964158";
        char *to_number = NULL;
        char *picture_url = NULL;
        char *c_name = NULL;
        opterr = 0;

        while ((cmd = getopt(argc, argv, "a:s:m:f:t:p:c:vh?")) != -1) {
                switch (cmd) {
                        case '?':
                        case 'h':
                                printf("Twilio C SMS Example Help:\n");
                                printf("-a: Account\t\t"
                                       "(ex: -a \"ACXXXXX\")\n");
                                printf("-s: Auth Token\t\t"
                                       "(ex: -s \"your_token\")\n");
                                printf("-f: From Number\t\t"
                                       "(ex: -f \"+18005551212\")\n");
                                printf("-t: To Number\t\t"
                                       "(ex: -t \"+18005551212\")\n");
                                printf("-m: Message to send\t"
                                       "(ex: -m \"Hello, Mundo\")\n");
                                printf("-p: (Opt.) URL to Image\t"
                                       "(ex: -p \"Hello, Mundo\")\n");
                                printf("-c: Container Name\t"
                                        "(ex: -c \"my_container\")\n");
                                printf("-v: Verbose Mode\n");
                                printf("-h: This help dialog\n");
                                return 0;
                        case 'a':
                                account_sid = optarg;
                                break;
                        case 's':
                                auth_token = optarg;
                                break;
                        case 'm':
                                message = optarg;
                                break;
                        case 'f':
                                from_number = optarg;
                                break;
                        case 't':
                                to_number = optarg;
                                break;
                        case 'p':
                                picture_url = optarg;
                                break;
                        case 'c':
                                c_name = optarg;
                                break;
                        case 'v':
                                verbose = true;
                                break;
                        default:
                                abort();
                }
        }

        if ( !account_sid || !auth_token || !from_number
                || !to_number || !message || !c_name ) {
                fprintf(stderr,
                        "You didn't include all necessary inputs!\n"
                        "Call using -h for help.\n");
                return -1;
        }

        if (verbose) {
                printf("Sending a message with the Twilio C SMS Demo!\n"
                       "________________________________________"
                       "________________________________________\n");
        }

        while (1){
            int c_status = is_c_running(c_name);

            if (c_status == 1){
                printf("Container %s is running.\n", c_name);
            } else if (c_status == 0) {
                printf("Container %s is not running. Send SMS warning...\n", c_name);

                if (twilio_send_message(account_sid, auth_token, message, from_number, to_number, picture_url, verbose) ) {
                    printf("Message sent successfully.\n");    
                } else {
                    fprintf(stderr, "Message failed to send.\n");
                    return -1;
                }
            } else {
                fprintf(stderr, "Container check failed.\n");
                return -1;
            }

        sleep(6);

        }

    return 0;

}