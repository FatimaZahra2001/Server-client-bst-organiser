#include <ctype.h>
pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
void *downtime(){

	sleep(1);
	//TODO: 1st downtime: Do balanceTree here
    pthread_rwlock_wrlock(&lock);
    root_balanced = balanceTree(root);
    pthread_rwlock_unlock(&lock);
	sleep(1);
	//TODO: 2nd downtime: Do balanceTree here
    pthread_rwlock_wrlock(&lock);
    freeSubtree(root_balanced);
    root_balanced = NULL;
    root_balanced = balanceTree(root);
    pthread_rwlock_unlock(&lock);
	sleep(1);
	//TODO: 3rd downtime: Do balanceTree here
    pthread_rwlock_wrlock(&lock);
    freeSubtree(root_balanced);
    root_balanced = NULL;
    root_balanced = balanceTree(root);
    pthread_rwlock_unlock(&lock);
	return NULL;
}
void* ServeClient(char *client){

//	 TODO: Open the file and read commands line by line
    FILE *fp;
    fp = fopen(client, "r");
    char str[50];

    if(fp == NULL){
        printf("Error: could not open file %s", client);
    }

    while(!feof(fp)){
        fscanf(fp,"%s",str);

        if(strstr(str, "insertNode") != NULL){
            int value;
            char chh[50];
            fscanf(fp,"%s",chh);
            value = atoi(chh);
            pthread_rwlock_wrlock(&lock);
            root = insertNode(root, value);
            printf("[%s]insertNode %d\n",client,value);
            pthread_rwlock_unlock(&lock);
        }
            // print: "[ClientName]insertNode <SomeNumber>\n"
            // e.g. "[client1_commands]insertNode 1\n"

            // TODO: Handle command: "deleteNode <some unsigned int value>"

        else if(strstr(str, "deleteNode") != NULL){
            int value;
            char chh[50];
            fscanf(fp,"%s",chh);
            value = atoi(chh);
            pthread_rwlock_wrlock(&lock);
            root = deleteNode(root, value);
            printf("[%s]deleteNode %d\n",client,value);
            pthread_rwlock_unlock(&lock);
        }

            // print: "[ClientName]deleteNode <SomeNumber>\n"
            // e.g. "[client1_commands]deleteNode 1\n"

            // TODO: Handle command: "countNodes"
        else if(strstr(str, "countNodes") != NULL){
            pthread_rwlock_rdlock(&lock);
            int value = 0;
            value = countNodes(root);
            printf("[%s]countNodes %d\n",client, value);
            pthread_rwlock_unlock(&lock);
        }
            // print: "[ClientName]countNodes = <SomeNumber>\n"
            // e.g. "[client1_commands]countNodes 1\n"

            // TODO: Handle command: "sumSubtree"
        else if(strstr(str, "sumSubtree") != NULL){
            pthread_rwlock_rdlock(&lock);
            int value = 0;
            value = sumSubtree(root);
            printf("[%s]sumSubtree %d\n",client, value);
            pthread_rwlock_unlock(&lock);
        }

    }
	// TODO: match and execute commands

	// TODO: Handle command: "insertNode <some unsigned int value>"

	// print: "[ClientName]sumSubtree = <SomeNumber>\n"
	// e.g. "[client1_commands]sumSubtree 1\n"
    fclose(fp);
	return NULL;
}
