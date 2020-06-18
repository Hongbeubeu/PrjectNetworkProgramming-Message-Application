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
        Server:
            typeData = 0 ; payLoad[] = 'user1 user2 user3'; (list name of user online)
            typeData = 1 ; payLoad[] = 'group1 group2 group3'; (list of group belong to this user)        
Sign up:
    typeMess = 1;
        Client:
            typeData = 0 ; payLoad[] = username;
            typeData = 1 ; payLoad[] = password;
        Server:
            typeData = 0 ; payLoad[] = NULL; (sign up success)
            typeData = 1 ; payLoad[] = NULL; (sign up fail, this acount existed)
Log in:
    typeMess = 2;
        Client:
            typeData = 0 ; payLoad[] = username;
            typeData = 1 ; payLoad[] = password;
        Server:
            typeData = 0; payLoad[] = NULL; (log in success)
            typeData = 1; payLoad[] = NULL; (Login failed, enter wrong password)
            typeData = 2; payLoad[] = NULL; (Login failed, this acount is blocked)
            typeData = 3; payLoad[] = NULL; (Login failed, the account is logged in elsewhere)
            typeData = 4; payLoad[] = NULL; (Login failed, this acount not exist)
Log out:
    typeMess = 3;
        Client:
            typeData = 0 ; payLoad[] = username;
        Server:
            typeData = 0 ; payLoad[] = NULL; (log out success)
            typeData = 1 ; payLoad[] = NULL; (log out fail)
    
Chat 1 - 1:
    typeMess = 4;
        Client:
            typeData = 0 ; payLoad[] = username; (username to chat)
            typeData = 1 ; payLoad[] = mess;
            typeData = 2 ; payLoad[] = NULL; (out of chat)
            typeData = 3 ; payLoad[] = NULL; (show more mess in history)
        Server:
            typeData = 1 ; payLoad[] = mess;
            typeData = 2 ; payLoad[] = NULL; (your friend out of chat)
    
Chat group
    typeMess = 5:
        Client:
            typeData = 0 ; payLoad[] = nameOfNewGroup;
            typeData = 1 ; payLoad[] = nameOfUserBeInvited;
            typeData = 2 ; payLoad[] = '1' (accept of invite) or '0' (not accept of invite);
            typeData = 3 ; payLoad[] = nameOfGroupToChat;
            typeData = 4 ; payLoad[] = mess;
            typeData = 5 ; payLoad[] = NULL; (out of chat)
            typeData = 5 ; payLoad[] = NULL; (see more mess)
        Server:
            typeData = 0 ; payLoad[] = '0' or '1'; ('0' mean create group success and '1' is opposite)
            typeData = 1 ; payLoad[] = 'username nameOfGroupToJoin' ; (first contain name of peple sent invite and second is name of group to join)
            typeData = 2 ; payLoad[] = username ; (name of user agree join group)
            typeData = 4 ; payLoad[] = mess;
            typeData = 5 ; payLoad[] = username; (name of user out of chat)

