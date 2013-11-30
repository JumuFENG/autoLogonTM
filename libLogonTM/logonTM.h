void logonTM(const char* QQAccount);
void addAQQRec(const char* QQAccount, const char* QQPwd);
// return the length of the real result. if the return value is bigger or equals the buflen, the string is not right.
// char* p = new char[buflen];
// int reallen = getAllAccounts(p, buflen);
// if( reallen >= buflen)
// {
//     delete[] p;
//     p = new char[reallen+1];
//     getAllAccounts(p, reallen+1);
// }
// dosomething with *p;
// delete[] p;
// that's the best way to use the interface below.
int getAllAccounts(char* rlt, int buflen);
void setQPath(const char* qpath);
int getLastPath(char* rlt, int buflen);
