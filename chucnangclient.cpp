/*
### Client:
    User Interface:
        - Hiển thị menu
            +   Bao gồm có 2 chức năng đăng kí và đăng nhập
        - Hiển thị home interface:
            +   Log out
            +   Create group
            +   Danh sách user online
            +   Danh sách group của user đó
            +   Danh sách thống báo các tin nhắn mới (làm sau )
            +   Lời mời tham ra group (làm sau)

        - Hiển thị Chat 1-1:
            +   Xem thêm tin nhắn (Có bắt sự kiện của chức năng)
            +   Bách về home (có bắt sự kiện của chức năng)
            +   20 tin nhắn gần nhất ( Khởi tạo )
            +   Các mess gửi đi và các mess nhận được
            +   Thông báo use kia out nếu có 
        - Hiển thị chat group:
            +   Hiển thị chắc năng back về home interface (Có bắt sự kiện cho chức năng)
            +   Hiển thị chắc năng mời user vào group (có bắt sự kiện cho chức năng)
            +   20 tin nhắn gần nhất trong group
            +   Các mess gửi đi và nhận được
            +   Xem thêm tin nhắn (có bắt sự kiện cho chức năng)


        - Nhận dữ liệu từ người dùng(ở các chức năng mà người dùng muốn thực hiện): 
            +   Home interface:
                * Bắt chức năng đăng xuất
                * Bắt chắc năng tạo nhóm
                * Chọn username or group để chat 
                * xem tin nhắn mới đồng thời đi vào chat với đối tượng đó luôn
                * Trả lời các thông báo mời vào nhóm
            +   Chat 1-1:
                * Thoát về home interactive
                * Nhận mess để gửi
            +   chat group:
                * Back về home interactive
                * Bắt sự kiện mời user vào group và lấy thông tin
                * Nhận vào tin nhắn để gửi đi
        - printResult
    Bussiness:
        - signup
            * Nhận username và password
        - login
        - logout
        - handleChatUser
        - handleChatGroup
        - handleHomeInterface
        - getAndCheckDataFromUser
    Data I/O: 
        - Communicate with server
        - craffMess

*/
int status, signResult, numOfUserOnl, numOfGroup;
char listOfUserOnl[10000];
char listOfGroup[1000];
status = 1
while(true){
    switch(status):
        case 1:
            menu(status, signResult);
            break;
        case 2: 
            homeInterface();
            break;
        case 3:
            chatUserInterface():
            break;
        case 4:
            chatGroupInterface();
            break;
    if(status == 1)
        printResult();
}

menu(int* status,int* signResult){
    char buff[1000], int typeMess, typeData;
    int choice; (1 là sigup, 2 là login)
    /* Hiển thị 2 chức năng login và sig up cho người dùng lựa chọn và nhận dữ liệu theo từng chức năng */
    choice = getAndCheckDataFromUser(buff, &typeMess, &typeData)
        switch(choice)
            case 1:
                signup(signResult));
                *status = 1;
                break;
            case 2:
                if(login(signResult) == true;
                    *status = 2;
                else
                    *status = 1
                break;
}

choiceOfUser = 0 in homepage, = 1 logout, = 2 tao nhom, = 3 chon user hoac gruop de chat
homeInterface(int* status){
    char buff[1000], int typeMess = 0, typeMess = 0;
    int choiceOfUser = 0;

    /* Hiển thị một số hướng dẫn bắt sự kiện: log out, tạo nhóm  */ 
    handleHomeInterface(choiceOfUser, typeMess, typeMess, NULL);
    int index = 1;
    cout << "List of user onl:" << endl;
    char* username = strtok(listOfUserOnl," ");
    while(username != NULL)
    {
        cout << "index: " << index << " ,name of user:" << username  << endl;
        ++index
        pch = strtok(NULL, " ");
    }
    char* group = strtok(listOfGroup," ");
    while(group != NULL)
    {
        cout << "index: " << index << " ,name of user:" << username  << endl;
        ++index
        pch = strtok(NULL, " ");
    }
    /* Bộ bắt sự kiện */
    
    while(1){
        cout << "indexOfUser:" ;
        cin >> choiceOfUser;
    }

    handleHomeInterface(choiceOfUser, typeMess, typeMess, buff);
    if(choiceOfUser == 1 || choice == 2)
        printResult();
}

handleHomeInterface(int choice, int typeMess, int typeData, char* buff){
    message mess;

    switch (choice):
        case 0:
            typeMess = 0;
            typeData = 0;
            buff = NULL;
            break
        case 1:
            typeMess = 3;
            typeData = 0; 
            buff = NULL;
            *status = 1;
            break;
        case 2:
            typeMess = 5;
            typeData = 0;
            *status = 2;
            break;
        case 3:
            {
                int index = 1;
                char* username = strtok(listOfUserOnl," ");
                while(username != NULL && index <= choiceToChoice)
                {
                    if(index == choiceToChat){
                        memcpy(buff, username);
                        typeMess = 4;
                        typeData = 0;
                        *status = 3;
                        break;
                    }
                    ++index
                    username = strtok(NULL, " ");
                }
                char* group = strtok(listOfGroup," ");
                while(group != NULL && index <= choiceToChoice)
                {
                    if(index == choiceToChat){
                        memcpy(buff, username);
                        typeMess = 5;
                        typeData = 3;
                        status = 4;
                        break;
                    }
                    ++index
                    pch = strtok(NULL, " ");
                }
            }
            break;
    craffMess(*mess, typeMess, typeMess, buff);
    senData();
    switch (choice)
    {
    case 0:
        buff = recvData();
        // kiem tra va gan usename vao listOfUserName
        buff = recvData();
        // Kiem tra va gan list group vao listOfGroup
        break;
    
    default:
        break;
    }
}

// choiceOfUser: = 0 back home, = 1 see more mess, = 2  chat
chatUserInterface(int* status){
    char buff[1000], int typeMess = 0, typeMess = 0;
    int choiceOfUser = 0;
    /* Hiển thị tin nhắn khởi tạo */
    /* Hiển thị một số hướng dẫn bắt sự kiện: back home, see more message  */
    while(something to beak out base on catch envent){
        /* Bắt sự kiện, nếu không băt sự kiện sẽ là chat thông thường, phần này có nhiệm vụ gán giá trị cho choiceOfUser */
        
        handleChatUser(choiceOfUser, typeMess, typeData, buff);
    }
}
handleChatUser(int choice, int typeMess, int typeData, char* buff){

    message mess;
    switch(chice):
        case 0:
            typeMess = 0;
            typeData = 0;
            buff = NULL;
            *stauts = 1;
            break;
        case 1:
            typeMess = 4;
            typeData = 3; 
            buff = NULL;
            break;
        case 2:
            typeMess = 4;
            typeData = 1;
            break;
    
    craffMess(*mess, typeMess, typeMess, buff);
    WSASend();
}

// choice = 0 backhome, = 1 see more message, 2 invite user
chatGroupInterface(int* status){
    char buff[1000], int typeMess = 0, typeMess = 0;
    int choiceOfUser = 0;
    /*Hiển thị tn khởi tạo */
    /* Hiển thị một số hướng dẫn bắt sự kiện: back home, see more message, invite user to group */

    while(something to beak out base on catch envent){
        /* Bắt sự kiện, nếu không băt sự kiện sẽ là chat thông thường, phần này có nhiệm vụ gán giá trị cho choiceOfUser */
        handleChatGroup(choiceOfUser, typeMess, typeData, buff);
    }
}
handleChatGroup(int choice, int typeMess,int typeData, char* buff){
    message mess;
    switch(choice):
        case 0:
            typeMess = 0;
            typeData = 0;
            buff = NULL;
            break;
        case 1:
            typeMess = 5;
            typeData = 5;
            buff = NULL;
            break;
        case 2:
            typeMess = 5;
            typeData = 1;
            break;
    craffMess(*mess, typeMess, typeMess, buff);
    WSASend();
}

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
            if

    

}


1       Sign up success
2       Sign up fail, this acount existed
3		Logged in successfully
4		Login failed, the account is logged in elsewhere
5		Login failed, enter wrong password
6		Login failed, this acount  is blocked
7		Login failed, this acount not exist
8		The acount are logging, can not logging again
