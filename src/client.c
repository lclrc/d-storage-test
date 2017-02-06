#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include "../../fastdfs-5.05/client/client_func.h"
#include "../../fastdfs-5.05/client/fdfs_client.h"
#include "../inc/make_log.h"

#define CONFIG_PATH "/etc/fdfs/client.conf"
// #define CONFIG_PATH "/etc/fdfs/client_home.conf"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("./client filename\n");
        exit(1);
    }

    int pipefd[2] = { 0 };
    int ret = 0;
    ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t cpid = fork();
    if (cpid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
#if 0
        execlp("fdfs_upload_file", "upload", "/etc/fdfs/client.conf", argv[1], NULL);
#else
        ConnectionInfo *pTrackerServer;
        ConnectionInfo storageServer;
        int store_path_index;
        char file_id[128] = { 0 };
        char group_name[1024] = { 0 };
        *group_name = '\0';

        fdfs_client_init(CONFIG_PATH);

        pTrackerServer = tracker_get_connection();

        tracker_query_storage_store(pTrackerServer, &storageServer, group_name, &store_path_index);

        storage_upload_by_filename1(pTrackerServer, &storageServer,
                                         store_path_index, argv[1], NULL,
                                         NULL, 0, group_name, file_id);
        printf("%s\n", file_id);

        tracker_disconnect_server_ex(pTrackerServer, true);

        fdfs_client_destroy();
#endif
    } else {
        /*
         * close(pipefd[1]);
         * int len = 0;
         * char buf[1024] = { 0 };
         * len = read(pipefd[0], buf, 1024);
         * buf[len-1] = '\0';
         * LOG("test", "fileid", "fileid: %s", buf);
         */
        close(pipefd[1]);
        int len = 0;
        char buf[1024] = { 0 };
        len = read(pipefd[0], buf, 1024);
        buf[len - 1] = '\0';
        LOG("test", "file_id", "file_id: %s", buf);
    }

    return 0;
}
