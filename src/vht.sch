#ifndef VHT_H
#define VHT_H
#include <libstring.h>

#define HOSTS_FP "/etc/hosts"
#define APACHE_VHOST_DIR "/etc/apache2/sites-available"
#define VHOST_TEMPLATE "<VirtualHost *:80>\n\n" \
	"\tServerName [hostname]\n" \
	"\tDocumentRoot [directory]\n\n" \
	"\t<Directory [directory]>\n" \
		"\t\tOptions Indexes FollowSymLinks\n" \
		"\t\tAllowOverride All\n" \
		"\t\tRequire all granted\n" \
		"\t\tAllow from all\n" \
	"\t</Directory>\n\n" \
"</VirtualHost>"

struct vht::s {
	string::string hostname;
	string::string directory;
};

static void usage();
static void process(const struct vht::s *session);


#endif
