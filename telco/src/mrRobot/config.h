#ifndef CONFIG_H
#define CONFIG_H


#define DEFAULT  "\033[39m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

#define ERASE   "\033[2K\r"


#define FORWARD_CHAR      'z'
#define BACKWARD_CHAR     's'
#define LEFT_CHAR         'q'
#define RIGHT_CHAR        'd'
#define STOP_CHAR         ' '
#define DISPLAY_LOG_CHAR  'r'
#define CLEAR_LOG_CHAR    'e'
#define QUIT_CHAR         'a'
#define NB_CHAR            8


#define DEFAULT_POWER    0
#define NOMINAL_POWER   50


#define SERVER_IP "localhost"
#define SERVER_PORT (2020)


#endif /* CONFIG_H */
