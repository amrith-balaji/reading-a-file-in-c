#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* replaceWord(const char* s, const char* oldW,
                const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
 
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
 
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
 
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
 
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
 
    result[i] = '\0';
    return result;
}
int main(int argc , char* argv[])
{
	FILE* ptr;
	char str[500];
	char *ret;
	char *result;
	int rankflag = 0;
	char symbol[100];
	char datestr[50];
	char rankstr[50];
	char price[100];
	ptr = fopen( argv[1], "r");
	if (NULL == ptr) {
		printf("file not found : %s \n", argv[1]);
		return 1;
	}
	while (fgets(str, 500, ptr) != NULL) {
		ret = strstr(str,"<title>" ) ;
		if (ret != NULL) {
			result = replaceWord(str , "Zacks", "");
			result = replaceWord(result , "    <title>", "");
			result = replaceWord(result , "</title>", "");
			result = replaceWord(result , " - Stock Price Today -", "");
                        result[strlen(result)-1] = '\0';
			strcpy(symbol,result);
		}
		ret = strstr(str,"<p class=\"last_price") ;
		if (ret != NULL) {
			result = replaceWord(str , "<span>", "");
			result = replaceWord(result , "</span>", "");
			result = replaceWord(result , "</p>", "");
			result = replaceWord(result , "            <p class=\"last_price\">", "");
			result = replaceWord(result , " USD", "");
			result = replaceWord(result , "$", "");
                        result[strlen(result)-1] = '\0';
			strcpy(price,result);	
//			printf(">>>>>> amrith pricestr %s",price);
		}
		ret = strstr(str," <input type=\"hidden\" class=\"to_date") ;
		if (ret != NULL) {
			result = replaceWord(str,"        <input type=\"hidden\"","");
			result = replaceWord(result , "class=\"to_date\" value=\"", "");
			result = replaceWord(result , "\" />", "");
                        result[strlen(result)-1] = '\0';
			strcpy(datestr,result);
//			printf(">> amrith datestr = %s\n",datestr);
		}
		ret = strstr(str,"1-Strong Buy" ) ;
		if (ret != NULL) {
			rankflag = 1;
        		//printf("Strong Buy \n");
			strcpy(rankstr,"Strong Buy");
		}
		else {
			ret = strstr(str,"2-Buy" ) ;
			if (ret != NULL) {
			//	printf("Buy\n");
				strcpy(rankstr,"Buy");
				rankflag = 1;
			}
			else {
				ret = strstr(str,"3-Hold");
				if (ret != NULL) {
					rankflag = 1;
				//	printf("Hold\n");
					strcpy(rankstr,"Hold");
				}
				else {
					ret = strstr(str,"4-Sell");
					if (ret != NULL) {
						rankflag = 1;
					//	printf("Sell\n");
						strcpy(rankstr,"Sell");
					}
					else {
						ret = strstr(str,"5-Strong Sell");
						if (ret != NULL) {
							rankflag = 1;
						//	printf("Strong Sell\n");
							strcpy(rankstr,"Strong Sell");
						}
						else {
							ret = strstr(str,"5-Strong Sell");
							if (ret != NULL) {
								rankflag = 1;
								//printf("Strong Sell\n");
								strcpy(rankstr,"Strong Sell");
							}
						}
					}
				}	
			}	
		}
		if (rankflag == 0){
			strcpy(rankstr,"No rank");
		}
	}
		printf(" INSERT INTO ZACKS_SNP (NAME,PRICE,DATE,RANK) VALUES (");
        	printf("\"%s\",",symbol);
		printf("\"%s\",",price);
        	printf("\"%s\",",datestr);
        	printf("\"%s\"",rankstr);
		printf(" );\n ");
	fclose(ptr);
return 0;
}
