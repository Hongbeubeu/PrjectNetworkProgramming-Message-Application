Chức năng: 
    Đăng kí
    Đăng nhập
    Đăng xuất
    Tạo nhóm chat
    Chat nhóm
    Gửi yêu cầu tham gia nhóm đến người khác
    Tham gia, rời nhóm chat 

Thông điệp từ client:
    Đăng nhập, đăng xuất, đăng kí, tạo nhóm chat, yêu cầu chat, gửi yêu cầu tham gia nhóm chat 

struct message
{
    int typeMess;
    int typeData;
    char payLoad[1000];
};

Đinh dạng thông điệp cho từng giao thức
homeInteractive
    typeMess = 0;
        client:
            typeData = 0 ; payLoad[] = NULL; (user in home)
            typeData = 1 ; payLoad[] = NUL; (log out)
            typeData = 2 ; payLoad[] = nameOfNewGroup; (server chả về có tạo thành công hay không)
            typeData = 3 ; payLoad[] = username; (username to chat) (khi nhận được thông điệp này server chả về 20 mess gần nhận trong khung chat cho người dùng)
            typeData = 4 ; payLoad[] = nameOfGroupToChat;(khi nhận được thông điệp này server chả về 20 mess gần nhận trong khung chat cho người dùng)
        Server:
            typeData = 0 ; payLoad[] = 'user1 user2 user3'; (list name of user online)
            typeData = 1 ; payLoad[] = 'group1 group2 group3'; (list of group belong to this user)
            typeData = 4 ; payLoad[] = '0' or '1'; ('0' mean create group success and '1' is opposite)
       
Log in:
    typeMess = 1;
        Client:
            typeData = 0 ; payLoad[] = NULL;
            typeData = 1 ; payLoad[] = "username password";
        Server:
            typeData = 0; payLoad[] = NULL; (log in success)
            typeData = 1; payLoad[] = NULL; (Login failed, enter wrong password)
            typeData = 2; payLoad[] = NULL; (Login failed, this acount is blocked)
            typeData = 3; payLoad[] = NULL; (Login failed, the account is logged in elsewhere)
            typeData = 4; payLoad[] = NULL; (Login failed, this acount not exist)
Sign up:
    typeMess = 2;
        Client:
            typeData = 0 ; payLoad[] = NULL;
            typeData = 1 ; payLoad[] = "username password";
        Server:
            typeData = 0 ; payLoad[] = NULL; (sign up success)
            typeData = 1 ; payLoad[] = NULL; (sign up fail, this acount existed)

Chat 1 - 1:
    typeMess = 3;
        Client:
            typeData = 0 ; payLoad[] = mess;
            typeData = 1 ; payLoad[] = NULL; (out of chat)
            typeData = 2 ; payLoad[] = NULL; (show more mess in history)
        Server:
            typeData = 0 ; payLoad[] = mess;
            typeData = 1 ; payLoad[] = NULL; (your friend out of chat)
    
Chat group
    typeMess = 4:
        Client:
            typeData = 1 ; payLoad[] = 'nameO nameOfUserBeInvited';
            typeData = 2 ; payLoad[] = '0' (accept of invite) or '1' (not accept of invite);
            typeData = 4 ; payLoad[] = mess;
            typeData = 5 ; payLoad[] = NULL; (out of chat)
            typeData = 5 ; payLoad[] = NULL; (see more mess)
        Server:
            typeData = 1 ; payLoad[] = 'username nameOfGroupToJoin' ; (first contain name of peple sent invite and second is name of group to join)
            typeData = 2 ; payLoad[] = username ; (name of user agree join group)
            typeData = 4 ; payLoad[] = mess;
            typeData = 5 ; payLoad[] = username; (name of user out of chat)



//note typeMess = 5, typeData = 2 client gửi thông điệp chấp nhận vào group thì không biết chấp nhận vào group nào.

bool checkUserOnline(const char *){
    // check xem trong map session xem username có trong đó chưa?
}

void getlistUser(char * result){
    tao lấy ra user name viết cách với nhau 1 dấu cách; // cái này lấy trong map
}

void getListGroup(char * result){
    lấy ra 1 list group mà nó tham gia // cái này lấy trong database
}

void incTryTimes(const char *){

}

void resetTryTimes(const char *){

}

void removeSession(const char *){

}


map< int, socket >

list ra 1 danh sách 10 tin nhắn mới nhất trong database
lấy đc list id mà cùng trong 1 nhóm chat 
duyệt các id
1, 2, 4 là những thằng trong 1 nhóm 
if tồn tại map[2] và map với group chat đó thì gửi còn ko tồn tại thì ko
duyệt mảng id thì đc map[id] là socket cần gửi
send[] và lưu lại database