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



theadHandleRecvFromServer(LPVOID completionPortID){
    /* Lắng nghe recve từ GetQueuedCompletionStatus() */
    /* Nhận dữ liệu và đầy vào queue of struct chứa thông tin trong thông điệp của server * nếu là recv/

    /* nếu là send thì lại lắng nghe */

}

threadHandleDataFromServer(message* mess){
    message mess;
    /*get info from mess */
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