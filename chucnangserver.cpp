bussiness:
    - handelMessFromClient
    - handleMessLogin
    - handleMessLogout
    - handleMessSignup
    - handleMessHomeInterface
    - handleChatUser
    - handleChatGroup
    - getListUserOnl
    - checkUserLogin
    - checkChatUser
    - checkChatgroup
I/O data:
    - Communicatewith Client:
        + craffReq
        + sendMess
    - queryDatabase:
        + check acount in database exsit of not
        + save an acount to database
        + getListGroupOfUser
        + getMessFromUser
        + save mess to user chat 1-1 table
        + get mess from group
        + save mess to group 


typedef struct{

    int typeMess;
    int typeData;
    char payLoad[1000];

} MESS, *LPMESS;

#define MESS_SIZE sizeof(MESS)

// Structure definition
typedef struct {
	WSAOVERLAPPED overlapped;
	WSABUF dataBuff;
	char buffer[MESS_SIZE];
	int bufLen;
	int recvBytes;
	int sentBytes;
	int operation;
	SOCKET socket;

} PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

typedef struct {
	SOCKET socket;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;


queue <PER_IO_OPERATION_DATA> queueOfIoData; 




theadHandleRecvFromServer(LPVOID completionPortID){
    /* Lắng nghe recve từ GetQueuedCompletionStatus() */
    /* Nhận dữ liệu và đầy vào queue of struct chứa thông tin trong thông điệp của server * nếu là recv/

    /* nếu là send thì lại lắng nghe */

}


threadHandleDataFromServer(){

    MESS mess;
    PER_HANDLE_DATA perHandeData;
	PER_IO_OPERATION_DATA perIoData;
	LPPER_IO_OPERATION_DATA pointIoData;
	while (true)
	{
		wait();
		if (queueOfIoData.size() != 0) {
			pointIoData = &(queueOfIoData.front());
			perIoData.recvBytes = pointIoData->recvBytes;
			memcpy(perIoData.buffer, pointIoData->buffer, perIoData.recvBytes);
			perIoData.dataBuff.buf = perIoData.buffer;
			perIoData.dataBuff.len = perIoData.recvBytes;
			perIoData.sentBytes = 0;
			perIoData.operation = pointIoData->operation;
			perIoData.socket = pointIoData->socket;
			perHandeData.socket = perIoData.socket;
			queueOfIoData.pop();
			signal();
            memcpy(&mess, perIoData.buffer, MESS_SIZE);
            // biến mess là struct chứa thông tin mà client gửi lên
            // nếu cần thông tin của socket hiện tại đang sử lí thì dùng perIoData.socket
            int typeMess = mess.typeMess, typeData = mess.typeData;
            char* buff = mess.buff;
			switch(typeMess):
            case 0:
                handleMessHomeInterface();
                break;
            case 1:
                handleMessSignup();
                break;
            case 2:
                handleMessLogin();
                break;
            case 3 :
                handleMessLogout();
            case 4: 
                handleChatuser();
            case 5:
                handleChatGroup();
        }
		signal();
		
	}    
}


handleMessHomeInterface(){
    message mess;
    char buff[1000], int typeMess = 0, typeMess = 0; 
    buff = getListUserOnl() // Hàm này k vào ra dữ liệu, yều cầu phải có cách lưu những user onl để kiêm tra
                            // Chả về một arr char chứa tên username onl, mỗi username trong arr cách nhau 1 dấu cách
                            // xem chi tiết ở phần thiết kế thông điệp
    typeMess = 0; typeMess = 0;
    craffMess(*mess, typeMess, 
    typeMess, buff);
    WSASend();                     
    buff getListGroupOfUser(int numOfMess) // query database
    typeMess = 0; typeMess = 1;
    craffMess(*mess, typeMess, typeMess, buff);
    WSASend();
    
}