void initialize_catalog(){
	for(int i=0; i < NUM_PRODUCTS; i++){
		sprintf(catalog[i].description, "Poduct %d", i);
		catalog[i].price = (i+1) * 5.0;
		catalog[i].item_count = 2;
	}
}

void initialize_variables(){
	struct sockaddr_un sa;
    (void)unlink(SOCKETNAME);
    strcpy(sa.sun_path, SOCKETNAME);
    sa.sun_family = AF_UNIX;
    int fd_skt, fd_client; char buf[100]; 
    int written; ssize_t readb;
}

void client_working(){
	void initialize_variables();
	for(int i=0; i < NUM_CLIENTS; i++){
		int pid = fork();
		
		if(pid < 0){
			perror("fork");
			exit(EXIT_FAILURE);
		}
		
		if(pid == 0){
			float total = 0.0;
			int succesful_orders = 0;
			int failed_orders = 0;
			if ((fd_skt = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
             perror("Message from socket [client] : ");
             exit(-1); }        
        	printf ("[Client] ==> Socket %d has been created\n", fd_skt);
        	
        	while (connect(fd_skt, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
            	if (errno == ENOENT) {
                	sleep(1); continue; }
            	else {
             		perror("Message from connect [client]");
             		exit(-2); }}
            printf ("[Client] ==> Connection has been established .. let us write to server\n");
			
			srand(time(NULL)+ i);
			
			for(int j=0; j<MAX_ORDERS;j++){
				int product_description = rand() % MAX_PRODUCTS;
				written  = write(fd_skt,&product_description,sizeof(product_description));
				if (written==-1) {
             		perror("Message from write [client]");
             		exit(-3); }
        		else {
        			printf ("[Client] ==> %d bytes written to server\n", written);
        			
				}
				
		        printf ("[Client] ==> Now let us read from server\n");             
		        readb = read(fd_skt, buf, sizeof(buf));
		        if (readb==-1) {
		             perror("Message from read [client]");
		             exit(-4); } 
		        else {
		        	printf ("[Client] ==> %zd bytes read from server\n", readb);
		        	if(strcpm(buf,"Succesfull order")){
		        		succesful_orders++;
		        		total+=catalog[product_description].price;
					}
					else{
						failed_orders++;
						printf("Client %d: products unavailable ,request failed");
					}
				}     
		        close(fd_skt);
		        exit(0); 
		  	}
		  	
		}
		sleep(1);
	}
	void print_results(failed_orders,succesfull_orders,total);
}
			
	
	
void server_working(){
	void initialize_variables();
	for(int i = 0; i < NUM_CLIENTS; i++){
		int product_id;
		int succesfull_requests = 0;
		int failed_requests = 0;
		
		if ((fd_skt = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
             perror("Message from socket [server]");
             exit(-1); }    
             
        printf ("[Server] ==> Socket %d has been created\n", fd_skt);
        if (bind(fd_skt, (struct sockaddr *) &sa, sizeof(sa))){
            perror("Message from bind [server]");
            exit(-2); }    
        printf ("[Server] ==> Socket %d has been bound to address\n", fd_skt);                
        if (listen(fd_skt, 5)){
            perror("Message from listen [server]");
            exit(-3); }
        printf ("[Server] ==> Listening for incoming messages\n");    
        if ((fd_client = accept(fd_skt, NULL, 0))<0) {
            perror("Message from accept [server]"); 
            exit(-4); }
		if(read(fd_client,&product_id,sizeof(int))>0){
			succesfull_requests++;
			char response[100];
			if(product_id>=0 && product_id<=20){
				if(catalog[product_id].item_count>0){
					catalog[product_id].item_count--;
					sprintf(response,"Order successful");
				}
				else{
					sprintf(response,"Order failed");
				}
			}
			else{
				sprintf(response,"Invalid product ID");
			}
			write(fd_client,response,sizeof(response));
		}
		else{
			failed_requests++;
		}
		close(fd_skt);
        close(fd_client);
        exit(0);
        void print_results_requests(int succesfull_requests,int failed_requests);
}
				
void print_results_orders(int failed_orders,int succesfull_orders,int total){
	printf("%d products were requested, where %d products were bought, totaling %.2f euros",succesful_orders+failed_orders,succesful_orders,total);
}

void print_results_requests(int succesfull_requests,int failed_requests){
	printf("%d requests were made, where %d succeeded and %d failed",succesfull_requests+failed_requests,succesfull_requests,failed_requests);
}
	
