#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	const char *action = NULL, *devpath = NULL, *physdevpath = NULL, *mediastatus = NULL;
	int sd = -1;
	struct sockaddr_un serv_addr_un;

	int mode = 0;

	if(argc == 3)
	{
		action = argv[1];
		devpath = argv[2];
		mode = 1;

		char data[1024];

		snprintf(data, sizeof(data) - 1, "ACTION=%s", action);
		data[sizeof(data) - 1] = 0;
		printf("%s\n", data);

		printf("ID_TYPE=%s\n", getenv("ID_TYPE"));
		printf("DEVTYPE=%s\n", getenv("DEVTYPE"));
		printf("DEVNAME=%s\n", getenv("DEVNAME"));
		printf("ID_FS_TYPE=%s\n", getenv("ID_FS_TYPE"));
		printf("ID_BUS=%s\n", getenv("ID_BUS"));
		printf("ID_PART_ENTRY_UUID=%s\n", getenv("ID_PART_ENTRY_UUID"));
		printf("ID_VENDOR=%s\n", getenv("ID_VENDOR"));

		return 0;

	}
	else if (argc > 3)
	{
		action = argv[1];
		devpath = argv[2];
		physdevpath = argv[3];
	}
	memset(&serv_addr_un, 0, sizeof(serv_addr_un));
	serv_addr_un.sun_family = AF_LOCAL;
	strcpy(serv_addr_un.sun_path, "/tmp/hotplug.socket");
	sd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (sd >= 0)
	{
		if (connect(sd, (const struct sockaddr*)&serv_addr_un, sizeof(serv_addr_un)) >= 0)
		{
			char data[1024];

			if(mode == 1)
			{
				if (action && devpath)
				{
					snprintf(data, sizeof(data) - 1, "ACTION=%s", action);
					data[sizeof(data) - 1] = 0;
					printf("%s\n", data);
					//send(sd, data, strlen(data) + 1, 0);

					if (strcmp(action, "add") == 0)
					{

						// ID_TYPE=disk
						// DEVTYPE=partition
						// ID_FS_TYPE=ext4
						// ID_MODEL=SAMSUNG_SSD_PM810_TH_64GB
						// ID_PART_ENTRY_UUID=d835c198-01
						// ID_VENDOR=JMicron ( Optional )
						// ID_BUS=ata , ID_BUS=usb
						// DEVNAME=/dev/sdd1


						printf("ID_TYPE=%s\n", getenv("ID_TYPE"));
						printf("DEVTYPE=%s\n", getenv("DEVTYPE"));
						printf("DEVNAME=%s\n", getenv("DEVNAME"));
						printf("ID_FS_TYPE=%s\n", getenv("ID_FS_TYPE"));
						printf("ID_BUS=%s\n", getenv("ID_BUS"));
						printf("ID_PART_ENTRY_UUID=%s\n", getenv("ID_PART_ENTRY_UUID"));
						printf("ID_VENDOR=%s\n", getenv("ID_VENDOR"));


//						snprintf(data, sizeof(data) - 1, "ACTION=%s", action);
//						data[sizeof(data) - 1] = 0;
//						send(sd, data, strlen(data) + 1, 0);
					}
				}
			}
			else 
			{

				if (!action) action = getenv("ACTION");
				if (action)
				{
					snprintf(data, sizeof(data) - 1, "ACTION=%s", action);
					data[sizeof(data) - 1] = 0;
					send(sd, data, strlen(data) + 1, 0);
				}
				else
				{
					mediastatus = getenv("X_E2_MEDIA_STATUS");
					if (mediastatus)
					{
						snprintf(data, sizeof(data) - 1, "X_E2_MEDIA_STATUS=%s", mediastatus);
						data[sizeof(data) - 1] = 0;
						send(sd, data, strlen(data) + 1, 0);
					}
				}

				if (!devpath)
				{
					devpath = getenv("DEVPATH");
					if (!devpath) devpath = "-";
				}
				snprintf(data, sizeof(data) - 1, "DEVPATH=%s", devpath);
				data[sizeof(data) - 1] = 0;
				send(sd, data, strlen(data) + 1, 0);
				if (!physdevpath)
				{
					physdevpath = getenv("PHYSDEVPATH");
					if (!physdevpath) physdevpath = "-";
				}
				snprintf(data, sizeof(data) - 1, "PHYSDEVPATH=%s", physdevpath);
				data[sizeof(data) - 1] = 0;
				send(sd, data, strlen(data) + 1, 0);

			}

		}
		close(sd);
	}
}
