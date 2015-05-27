%{
int obrace = 0;
char *temp;
int cnt = 0;
%}

DIGIT    [0-9]
ID       [a-zA-Z][a-zA-Z0-9\@_-]*
OBRACE   "{"
EBRACE   "}"
FILENAME [a-zA-Z0-9\/.-]+

%%
\/								{printf("/\n");}
"/"+{ID}+"/"			{;}
{FILENAME}				{;}
{ID}+[ \t]*+"{"		{obrace++;for (cnt = obrace; cnt > 0; cnt--) printf("-"); temp=strtok(yytext,"{"); printf("%s\n", temp);}
[ \t\n]+          /* eat up whitespace */
{EBRACE}					{obrace--;}
.									/**/
