#include "CommonHead.h"
#include "Lock.h"
#include "basemessage.h"
#include "comstruct.h"
#include "KernelDefine.h"
#include "INIFile.h"
#include "configManage.h"
#include "Function.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include "log.h"
#include "GServerConnect.h"
#include "DataLine.h"
#include "Util.h"
#include "MyCurl.h"
#include "Define.h"
#include "json/json.h"
#include "RedisCenter.h"
#include "Xor.h"
#include "NewMessageDefine.h"
#include "test.pb.h"
#include "ServerTimer.h"


using namespace test;

static timeval g_lasttime;
void* TimerFun(void* p)
{
	evutil_gettimeofday(&g_lasttime, NULL);
	CDataLine* pDataLine = (CDataLine*)p;
	//���ݻ���
	BYTE szBuffer[LD_MAX_PART] = "";
	DataLineHead* pDataLineHead = (DataLineHead*)szBuffer;

	while (true)
	{
		usleep(20000);

		while (pDataLine->GetDataCount())
		{
			unsigned int bytes = pDataLine->GetData(pDataLineHead, sizeof(szBuffer));

			struct timeval newtime, difference;
			double elapsed;
			evutil_gettimeofday(&newtime, NULL);
			evutil_timersub(&newtime, &g_lasttime, &difference);
			elapsed = difference.tv_sec +
				(difference.tv_usec / 1.0e6);

			ServerTimerLine* pTimerMessage = (ServerTimerLine*)pDataLineHead;

			printf("timeout_cb called at %ld: %.3f seconds elapsed. timer %u \n",
				newtime.tv_sec, elapsed, pTimerMessage->uTimerID);
			g_lasttime = newtime;
		}

	}
}

int main()
{
	//GOOGLE_PROTOBUF_VERIFY_VERSION;

	//Team team;
	//team.set_id(1);
	//team.set_name("Rocket");
	//Student* s1 = team.add_student(); // ���repeated��Ա
	//s1->set_id(1);
	//s1->set_name("Mike");
	//s1->set_sex(BOY);
	//Student* s2 = team.add_student();
	//s2->set_id(2);
	//s2->set_name("Lily");
	//s2->set_sex(GIRL);

	//// encode --> bytes stream
	//string out;
	//team.SerializeToString(&out);

	//// decode --> team structure
	//Team t;
	//t.ParseFromArray(out.c_str(), out.size()); // or parseFromString
	//cout << t.DebugString() << endl;
	//for (int i = 0; i < t.student_size(); i++) {
	//	Student s = t.student(i); // ��������repeated��Ա
	//	cout << s.name() << " " << s.sex() << endl;
	//}



	CUtil::MkdirIfNotExists("log/");
	CUtil::MkdirIfNotExists(SAVE_JSON_PATH);
	// ���÷���������
	ConfigManage()->SetServiceType(SERVICE_TYPE_LOADER);
	// �����������̵߳�log�ļ�
	GameLogManage()->AddLogFile(GetCurrentThreadId(), THREAD_TYPE_MAIN);
	ConfigManage()->Init();



	CGServerConnect ddd;
	CDataLine *dataline = new CDataLine;
	//ddd.Start(&dataline, 38);
	CServerTimer *pTime = new CServerTimer;
	pTime->Start(dataline);
	pTime->SetTimer(654321, 500);
	evutil_gettimeofday(&g_lasttime, NULL);
	printf("currtime %ld\n", g_lasttime.tv_sec);
	// �����߳�
	pthread_t threadID1 = 0;
	int err = pthread_create(&threadID1, NULL, TimerFun, (void*)dataline);
	if (err != 0)
	{
		SYS_ERROR_LOG("ThreadCheckTimer failed");
	}



	//pTime->Stop();


	////�����ʼ��ӿ�
	//MyCurl curl;
	//std::vector<std::string> vUrlHeader;
	//std::string postFields = "";
	//std::string result = "";
	////�������URL
	//std::string url = "http://api.androidhive.info/volley/person_object.json";
	//curl.postUrlHttps(url, vUrlHeader, postFields, result);
	//std::cout << result << endl;


	/*CRedisCenter* pRedis = new CRedisCenter;
	pRedis->Init();*/

	/*string strJsonContent = "{\"role_id\": 1,\"occupation\": \"paladin\",\"camp\": \"alliance\"}";
	int nRoleDd = 0;
	string strOccupation = "";
	string strCamp = "";
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strJsonContent, root))
	{
		nRoleDd = root["role_id"].asInt();
		strOccupation = root["occupation"].asString();
		strCamp = root["camp"].asString();
	}
	cout << "role_id is: " << nRoleDd << endl;
	cout << "occupation is: " << strOccupation << endl;
	cout << "camp is: " << strCamp << endl;*/

	/*LogonResponseLogon msg;
	strcpy(msg.name, "123456");
	msg.money = 963852741;
	Xor::Encrypt((unsigned char *)&msg, sizeof(msg));
	Xor::Decrypt((unsigned char*)& msg, sizeof(msg));*/


	/*
	int fd = 10;
	int ret;
	ret = close(fd);
	if (ret == -1)
	{
		SYS_ERROR_LOG("close fd");
		CON_INFO_LOG("====");
		CON_ERROR_LOG("====");
	}*/
	

	

	//struct timeval tv;
	//gettimeofday(&tv, NULL);
	//printf("second:%ld\n", tv.tv_sec);  //��
	//printf("millisecond:%ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000);  //����
	//printf("microsecond:%ld\n", tv.tv_sec * 1000000 + tv.tv_usec);  //΢��

	//sleep(3); // Ϊ����ۿ����ó���˯�����Ա�
	//std::cout << "3s later:" << std::endl;

	//gettimeofday(&tv, NULL);
	//printf("second:%ld\n", tv.tv_sec);  //��
	//printf("millisecond:%ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000);  //����
	//printf("microsecond:%ld\n", tv.tv_sec * 1000000 + tv.tv_usec);  //΢��


	//printf("+++++++++++++++==\n"); 
	//CSignedLock lock;
	//CSignedLockObject testLock(&lock, false);

	//testLock.Lock();
	//testLock.Lock();
	//printf("+++++++++++++++==\n");

	//testLock.UnLock();
	//testLock.UnLock();

	int    socket_fd, connect_fd;
	struct sockaddr_in     servaddr;
	char    buff[4096];
	int     n;
	//��ʼ��Socket  
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	//��ʼ��  
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP��ַ���ó�INADDR_ANY,��ϵͳ�Զ���ȡ������IP��ַ��  
	servaddr.sin_port = htons(6666);//���õĶ˿�ΪDEFAULT_PORT  

	//�����ص�ַ�󶨵����������׽�����  
	if (bind(socket_fd, (struct sockaddr*) & servaddr, sizeof(servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	//��ʼ�����Ƿ��пͻ�������  
	if (listen(socket_fd, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}
	printf("======waiting for client's request======\n");
	while (1) {
		//����ֱ���пͻ������ӣ���Ȼ���˷�CPU��Դ��  
		if ((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1) {
			printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
			continue;
		}
		//���ܿͻ��˴�����������  
		n = recv(connect_fd, buff, sizeof(buff), 0);
		//��ͻ��˷��ͻ�Ӧ����  
		if (!fork()) { /*�Ͻ���*/
			if (send(connect_fd, "Hello,you are connected!\n", 26, 0) == -1)
				perror("send error");
			close(connect_fd);
			exit(0);
		}
		buff[n] = '\0';
		printf("recv msg from client: %s\n", buff);
		close(connect_fd);
	}

	close(socket_fd);

    return 0;
}