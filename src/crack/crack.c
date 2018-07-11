#include <stdlib.h>
#include <crypt.h>
#include <string.h>
#include <stdio.h>

/*
 * Alphanumeric array to be iterated through for password cracking
 */
/*
char data[62] = {
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', \
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', \
	'1','2','3','4','5','6','7','8','9','0' \
};
*/
 char data[62] = {
     'f','l','u','y','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', \
     'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', \
     '1','2','3','4','5','6','7','8','9','0' \
 };


void interpretLine(char *line, int pwlen, char *passwd);

/*
 * Find the plain-text password PASSWD of length PWLEN for the user USERNAME 
 * given the encrypted password CRYPTPASSWD.
 */
void crackSingle(char *username, char *cryptPasswd, int pwlen, char *passwd) { 
	char *salt = malloc(2 * sizeof(char));
	char *tmpPswd = malloc (6* sizeof(char));
	char *tmpCryptPswd = malloc(sizeof(char*));

	strncpy(salt,username,2);

	for(int i = 0; i < 62; i++){
		printf("%c\n", data[i]);
		for(int j = 0; j < 62; j++){
			for(int k = 0; k < 62; k++){
				for(int l = 0; l < 62; l++){
					for(int m = 0; m < 62; m++){
						for(int n = 0; n < 62; n++){
							tmpPswd[0]= data[i];
							tmpPswd[1]= data[j];
							tmpPswd[2]= data[k];
							tmpPswd[3]= data[l];
							tmpPswd[4]= data[m];
							tmpPswd[5]= data[n];

							tmpCryptPswd = crypt(tmpPswd, salt);
                            // printf("%s : %s\n", tmpPswd, tmpCryptPswd);

							if(strcmp(tmpCryptPswd,cryptPasswd) == 0){
								strcpy(passwd, tmpPswd);
								free(salt);free(tmpPswd);
								return;
							}
						}
					}
				}
			}
		}
	}
}


/*
 * Find the plain-text passwords PASSWDS of length PWLEN for the users found
 * in the old-style /etc/passwd format file at pathe FNAME.
 */
void crackMultiple(char *fname, int pwlen, char **passwds) {
	FILE *fp;
	if((fp = fopen(fname,"r")) == NULL){
		printf("Failed to open file");
		exit(1);
	}

	char line[250];

	int i = 0;
	while(fgets(line,sizeof(line), fp) != NULL){

		// Using David Harrison's code
		// from test.c
	    char passwd[pwlen + 1];
	    memset(passwd, 0, pwlen + 1);


		interpretLine(line, pwlen, passwd);
		strcpy(passwds[i], passwd);
		i++;
	}

	fclose(fp);

} 

/*
 * Take a line from an /etc/passwd file and parse it for username and
 * encrypted password. Lines are assumed to be correctly formatted. 
 * See report for explaination.
 */
void interpretLine(char *line, int pwlen, char *passwd){
	char *username;
	char *encryptedPasswd;

	// tokenize substrings on delimeter ':'
	username = strtok(line, ":");
	encryptedPasswd = strtok(NULL, ":");

	crackSingle(username, encryptedPasswd, pwlen, passwd);

	return;
}


/*
 * Find the plain-text passwords PASSWDS of length PWLEN for the users found
 * in the old-style /etc/passwd format file at pathe FNAME.
 */
void crackSpeedy(char *fname, int pwlen, char **passwds) { 
	crackMultiple(fname, pwlen, passwds);
	return;
}

/*
 * Find the plain-text password PASSWD of length PWLEN for the user USERNAME 
 * given the encrypted password CRYPTPASSWD withoiut using more than MAXCPU
 * percent of any processor.
 */
void crackStealthy(char *username, char *cryptPasswd, int pwlen, char *passwd, int maxCpu) { }

