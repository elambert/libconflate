#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sysexits.h>

#include "conflate.h"

void display_config(void* userdata, kvpair_t* conf)
{
    printf("Hey.  I received a new config (userdata: %s):\n",
           (char*)userdata);

    while (conf) {
        int i = 0;
        printf("\t%s\n", conf->key);

        for (i = 0; conf->values[i]; i++) {
            printf("\t\t%s\n", conf->values[i]);
        }

        conf = conf->next;
    }
}

void do_stats(void* userdata, void* opaque, conflate_add_stat add_stat)
{
    add_stat(opaque, "stat1", "val1");
    add_stat(opaque, "stat2", "val2");
    add_stat(opaque, NULL, NULL);
}

void do_reset_stats(void* userdata)
{
    printf("Resetting stats...\n");
}

int main(int argc, char **argv) {

    conflate_config_t conf;

    if (argc < 3) {
        fprintf(stderr, "Usage: bot <jid> <pass> [host]\n");
        exit(EX_USAGE);
    }

    conf.jid = argv[1];
    conf.pass = argv[2];
    conf.host = (argc == 4 ? argv[3] : NULL);
    conf.software = "conflate sample bot";
    conf.version = "1.0";
    conf.save_path = "/tmp/conflate.db";

    conf.userdata = "something awesome";
    conf.new_config = display_config;
    conf.get_stats = do_stats;
    conf.reset_stats = do_reset_stats;

    if(!start_conflate(conf)) {
        fprintf(stderr, "Couldn't initialize libconflate.\n");
        exit(1);
    }

    pause();
}