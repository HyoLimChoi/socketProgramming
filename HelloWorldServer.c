
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
 
#define PORT 9000
#define BUFSIZE 100
#define SLEEPTIME 1
char buffer[BUFSIZE] = "hello, I'm server";
char rcvBuffer[BUFSIZE];
main( )
{
	int   c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int   len;
	int   n;
	int readSize;
 	s_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
 
	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}
 
	if(listen(s_socket, 5) == -1) {
		printf("listen Fail\n");
		return -1;
	}
 	
	while(1) {
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		printf("Client is Connected\n");
		while(1){		
			char * token=strtok(rcvBuffer," ");	

			memset(rcvBuffer, 0, BUFSIZE);
			if((readSize = read(c_socket, rcvBuffer, sizeof(rcvBuffer))) < 0) {
               	 		return -1;
        		}
			printf("Received Data From Client: %s\n", rcvBuffer);
			if(strcasecmp(rcvBuffer, "quit") == 0){
				break;
			}else if(strcasecmp(rcvBuffer, "hello") == 0){
				strcpy(buffer, "Hello, Nice to meet you");
			}else if(strcasecmp(rcvBuffer, "how are you?") == 0){
				strcpy(buffer, "Fine. Thank you. And you?");
			}else if(strcasecmp(rcvBuffer, "What's your name?") == 0){
				strcpy(buffer, "My name is hongkyu park.");
			}else if(strcasecmp(rcvBuffer, "Do you like sports?") == 0){
				strcpy(buffer, "Yes. I like soccer");
			}else if(strcasecmp(rcvBuffer, "hey man!?")==0){
				strcpy(buffer,"what?!");
			}else if(strcasecmp(rcvBuffer, "How old are you??")==0){
				strcpy(buffer,"Secret!haha");
			}else if(strcasecmp(rcvBuffer," I want IPhoneX")==0){
				strcpy(buffer,"me too");
			}else if(strncmp(rcvBuffer,"strlen",6)==0)
			{ //문자열 길이 출력 문
				int result=0;
				token = strtok(NULL, " "); //strlen 다음 입력한 문자열 확인을 위해 한번 자르기
				result += strlen(token); //strlen 다음 입력한 문자열 길이 확인
				token = strtok(NULL, " "); //그 다음 띄어쓰기 있는지 확인하기
		
				//만약에 띄어쓰기 다음이 존재한다면 그 문자열 길이 추가
				while (token != NULL) 
				{
					result += strlen(token);
					token = strtok(NULL, " ");
					result++; //여백크기 포함용
				}	
				sprintf(buffer,"문자열의 길이는 %d입니다.\n",result);
			}else if(strncmp(rcvBuffer, "strcmp", 6) == 0){ //두개의 문자열 비교
				int cnt = 0;
				int i=0;
				char temp[100], temp2[100];
				token = strtok(NULL, " "); //strcmp 다음 문자열 확인을 위해 자르기

				for (i = 0; i < 2; i++) {
					if (i == 0)
						strcpy(temp, token); //한번은 strcmp 바로 다음 문자열 
					else
						strcpy(temp2, token); //다음은 앞 문자열과 비교 할 문자열 복사
					token = strtok(NULL, " ");
				}
				if (strcmp(temp, temp2) == 0){
					sprintf(buffer,"%s와 %s는 같은 문자열입니다.\n",temp,temp2);
				}else
					sprintf(buffer,"%s와 %s는 다른 문자열입니다.\n",temp,temp2);

			}
			else if (strncmp(rcvBuffer, "readfile", 8) == 0) { //파일 읽기 용
				FILE * f;
				char temp[255];
				int cnt = 0;
				int i=0;
				token = strtok(NULL, " "); //readfile 다음 입력한 문자열 확인을 위해 자르기
				f = fopen(token, "r"); //"r" 파일읽기 호출

				if (f == NULL) //파일이 없을 시 없음 출력
					sprintf(buffer,"파일이 존재하지 않습니다.\n");
				else {
					while (1)
					{
						if (fscanf(f, "%s", temp) == EOF) 
						{
						break;
						}
					i += sprintf(buffer + i, "%s\n", temp);
					}
				    }
			}else {
				strcpy(buffer, "I don't understand what you say.");
			}
			n = strlen(buffer);
			sleep(SLEEPTIME);
			write(c_socket, buffer, n);
			printf("Send Data: %s\n", buffer);
			//write(c_socket, rcvBuffer, strlen(rcvBuffer));
			//printf("Send Data: %s\n", rcvBuffer);
		}
		close(c_socket);
	}	
	close(s_socket);
}
