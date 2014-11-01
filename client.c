#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#include "rtspType.h"
#include "rtspClient.h"
#include "tpool.h"

static void help(int status);
static void help(int status)
{
    printf("Usage: rpsp_client -u rtsp://url\n\n");
    printf("  -u, --url=rtsp://      rtsp address\n");
	printf("  -h, --help             print this help\n");
	printf("\n");
	exit(status);
}


int32_t main(int argc, char **argv)
{
    int32_t opt;
    char *url = NULL;
    static const struct option long_opts[] = {
                            { "url", required_argument, NULL, 'u'},
                            { "help", no_argument, NULL, 'h'},
                            { NULL, 0, NULL, 0 }
                        };

    while ((opt = getopt_long(argc, argv, "u:h",
                       long_opts, NULL)) != -1) {
        switch (opt) {
            case 'u':
                url  = strdup(optarg);
                break;
            case 'h':
                help(EXIT_SUCCESS);
                break;
            default:
                break;
        }
    }

    if (NULL == url){
        fprintf(stderr, "Error : Url Address Equal Null.\n");
        return 0x00;
    }

    RtspClientSession *cses = InitRtspClientSession();
    if ((NULL == cses) || (False == ParseRtspUrl(url, cses))){
        fprintf(stderr, "Error : Invalid Url Address.\n");
        return 0x00;
    }

    pthread_t rtspId = RtspCreateThread(RtspEventLoop, (void *)cses);
    if (rtspId < 0x00){
        fprintf(stderr, "RtspCreateThread Error!\n");
        return 0x00;
    }


    do{
        pthread_join(rtspId, NULL);
        break;
    }while(0);

    printf("RTSP Event Loop stopped, waiting 5 seconds...\n");
    DeleteRtspClientSession(cses);
    return 0x00;
}


