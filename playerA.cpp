#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;
#define NOT_FIND ="not find"
struct Fire
{
    float x;
    float y;
};

struct Potion
{
    float x;
    float y;
};
int ismap=0;
map<string,int> Medic_map;
map<string,int> Gunner_map;
map<string,int> Hurler_map; //类型映射
vector<vector<int>> game_map; //游戏地图
pair<float,float> GunnerA_Pos; //GunnerA位置
pair<float,float> HurlerA_Pos; //HurlerA位置
pair<float,float> MedicA_Pos; //MedicA位置
pair<float,float> GunnerB_Pos; //GunnerB位置
pair<float,float> HurlerB_Pos; //HurlerB位置
pair<float,float> MedicB_Pos; //MedicB位置
float GunnerA_HP; //GunnerA血量
float HurlerA_HP; //HurlerA血量
float MedicA_HP;  //MedicA血量
float GunnerB_HP; //GunnerB血量
float HurlerB_HP; //HurlerB血量
float MedicB_HP;  //MedicB血量
bool has_coin; //金币存在信息
pair<float,float> coin_Pos; //金币位置信息
int scoreA; //A阵营得分
int scoreB; //B阵营得分
vector<Fire> fire_pos; //场上火焰的位置
vector<Potion> potion_pos; //场上药水的位置

int frame_count; //帧数信息

void init_input()
{
    string init_str;
    cin >> init_str;

    int n,m;
    cin >> n >> m;
    for (int i = 0; i < n; i++)
    {
        vector<int> temp(m,0);
        for (int j = 0; j < m; j++)
        {
            cin >> temp[j];
        }
        game_map.push_back(temp);
    }
    
    cin >> init_str;
}

void soldier_input()
{
    string pos;
    cin >> pos;
    int count;
    cin >> count;
    for (int i = 0; i < count; i++)
    {
        string team;
        string type;
        float hp;
        float x;
        float y;
        cin >> pos >> team >> pos >> type >> pos >> hp >> pos >> x >> pos >> y;
        if (team == "A")
        {
            if (type == "GunnerA")
            {
                GunnerA_HP = hp;
                GunnerA_Pos.first = x;
                GunnerA_Pos.second = y;
            }else if (type == "HurlerA")
            {
                HurlerA_HP = hp;
                HurlerA_Pos.first = x;
                HurlerA_Pos.second = y;
            }else if (type == "MedicA")
            {
                MedicA_HP = hp;
                MedicA_Pos.first = x;
                MedicA_Pos.second = y;
            }
        }else if (team == "B")
        {
            if (type == "GunnerB")
            {
                GunnerB_HP = hp;
                GunnerB_Pos.first = x;
                GunnerB_Pos.second = y;
            }else if (type == "HurlerB")
            {
                HurlerB_HP = hp;
                HurlerB_Pos.first = x;
                HurlerB_Pos.second = y;
            }else if (type == "MedicB")
            {
                MedicB_HP = hp;
                MedicB_Pos.first = x;
                MedicB_Pos.second = y;
            }
        }
    }
    cin >> pos;
}

void fire_input()
{
    fire_pos.clear();
    string pos;
    cin >> pos;
    int count;
    cin >> count;
    
    for (int i =0; i < count; i++)
    {
        Fire fire;
        float x;
        float y;
        cin >> pos >> x >> pos >> y;
        fire.x = x;
        fire.y = y;
        fire_pos.push_back(fire);
    }

    cin >> pos;
}

void score_input()
{
    string pos;
    cin >> pos;

    int A;
    int B;
    cin >> pos >> A >> pos >> B;
    scoreA = A;
    scoreB = B;

    cin >> pos;
}

void coin_input()
{
    string pos;
    cin >> pos;
    
    string temp_has_coin;
    cin >> temp_has_coin;
    
    if (temp_has_coin == "true")
    {
        float x;
        float y;
        cin >> pos >> x >> pos >> y;
        coin_Pos.first = x;
        coin_Pos.second = y;
        has_coin = true;
    }else if (temp_has_coin == "false")
    {
        has_coin = false;
        cout << "Dont have coin!!!" << endl;
    }
    
    cin >> pos;
}

void potion_input()
{
    potion_pos.clear();
    string pos;
    cin >> pos;

    int count;
    cin >> count;
    for (int i = 0; i < count; i++)
    {
        Potion potion;
        float x;
        float y;
        cin >> pos >> x >> pos >> y;
        potion.x = x;
        potion.y = y;
        potion_pos.push_back(potion);
    }

    cin >> pos;
}

void per_frame_input()
{
    string frame;
    cin >> frame >> frame_count;

    soldier_input();
    fire_input();
    score_input();
    coin_input();
    potion_input();

    cin >> frame >> frame_count;
}

//函数使用说明使用find_enemy函数作用是：发现周围血量最少的敌军，无传入参数，返回值是敌方的名字，为string类型
//如果三个人的视野中均无地方，则返回宏NOT_FIND，判断为find_enemy()==NOT_FIND就行
// string find_enemy(){
//     bool flag=false;
//     ifloat blood=0;
//     string name;
//     if(GunnerB_Pos.first!=0||GunnerB_Pos.second!=0){
//         name="GunnerB";
//         blood=GunnerB_HP;
//         flag=true;
//     }
//     if(HurlerB_Pos.first!=0||HurlerB_Pos.second!=0){
//         if(flag==false){
//             name="HurlerB";
//             blood=GunnerB_HP;
//             flag=true;
//         }
//         else{
//             if(HurlerB_HP<blood){
//                 name="HurlerB";
//                 blood=HurlerB_HP;
//             }
//         }
//     }
//     if(MedicB_Pos.first!=0||MedicB_Pos.second!=0){
//         if(flag==false){
//             name="MedicB";
//             blood=Medic_HP;
//             flag=true;
//         }
//         else{
//             if(MedicB_HP<blood){
//                 name="MedicB";
//                 blood=MedicB_HP;
//             }
//         }
//     }
//     if(flag==true){
//         return name;
//     }
//     else{
//         return "not find";
//     }
// }


void shoot(string name, float pos_x, float pos_y){
    cout<<"shoot"<<" "<<name<<" "<<pos_x<<" "<<"pos_y";
}

void move(string man,float x,float y){                  //让man移动到x,y
    cout << "move "<<man<<" "<< x << " " << y << endl;
}
bool isclose(string name,pair<float,float> pos,float x,float y){    //判断pos是否在x,y附近
    if(abs(pos.first-x)<=0.3&&abs(pos.second-y)<=0.3){
        // move(name,pos.first-0.5,pos.second-0.5);
        // cerr<<"move to"<<pos.first-0.5<<" "<<pos.second-0.5<<endl;
        return true;
    }
    return false;
}

void print_pos(string name){
    if (name=="GunnerA")
    {
        cerr<<"GunnerA_Pos:"<<GunnerA_Pos.first<<" "<<GunnerA_Pos.second<<endl;
    }else if(name=="HurlerA"){
        cerr<<"HurlerA_Pos:"<<HurlerA_Pos.first<<" "<<HurlerA_Pos.second<<endl;
    }else if(name=="MedicA"){
        cerr<<"MedicA_Pos:"<<MedicA_Pos.first<<" "<<MedicA_Pos.second<<endl;
    }
    return ;
}
void output_command() //请不要删除End一行，每行输出记得加上换行符
{
    if(HurlerA_HP>0){
        if(frame_count%1250){
        cout<<"skill hurler 1 28.5 26.5"<<endl;
        }

        //hurler移动到4.5,16.5
        if(Hurler_map["flag1"]==0){
            move("hurler",4.5,16.5);
            Hurler_map["flag1"]=1;
        }
        

        //hurler移动到12,21.5
        if(Hurler_map["flag2"]==0&&isclose("HurlerA",HurlerA_Pos,4.5,16.5)){
            move ("hurler",15.5,16.5);
            Hurler_map["flag2"]=1;
        }

        //打印hurler的位置
        if((frame_count%100)==0){
            print_pos("HurlerA");
        }

        //

        //hurler释放技能
        if(Hurler_map["flag3"]==0&&isclose("HurlerA",HurlerA_Pos,12,21.5)){
            // cout<<"skill hurler 1 38 38"<<endl;
            move("hurler",12,22.5);
            Hurler_map["flag3"]=1;
        }

        //hurler移动到16.5,22.5
        if(Hurler_map["flag4"]==0&&isclose("HurlerA",HurlerA_Pos,12,22.5)){
            move("hurler",16.5,22.5);
            Hurler_map["flag4"]=1;
        }

        //hurler释放技能到26.5，25.5
        // if(Hurler_map["flag5"]==0&&isclose("HurlerA",HurlerA_Pos,16.5,22.5)){
        //     cout<<"skill hurler 1 26.5 25.5"<<endl;
        //     Hurler_map["flag5"]=1;
        // }

        //刷新hurler技能
        // if(frame_count%750==0){
        //     Hurler_map["flag5"]=0;
        // }

        //hurler去找金币
        if(has_coin&&Hurler_map["flag6"]==0&&isclose("HurlerA",HurlerA_Pos,16.5,22.5)){
            move("hurler",16.5,21.5);
            Hurler_map["flag6"]=1;
            cerr<<"aaaaaa"<<endl;
        }

        if(has_coin&&Hurler_map["flag7"]==0&&(isclose("HurlerA",HurlerA_Pos,16.5,21.5)||isclose("HurlerA",HurlerA_Pos,15.5,16.5))){
            move("hurler",coin_Pos.first,coin_Pos.second);
            Hurler_map["flag7"]=1;
            cerr<<"get coin"<<endl;
        }

        //金币刷新
        if(has_coin==true&&Hurler_map["flag9"]==1){
            Hurler_map["flag6"]=0;
            Hurler_map["flag7"]=0;
            Hurler_map["flag8"]=0;
            Hurler_map["flag9"]=0;
        }

        //hurler跑回去
        if(Hurler_map["flag8"]==0&&!has_coin){
            move("hurler",16.5,21.5);
            cerr<<"cccccc"<<endl;
            Hurler_map["flag8"]=1;
        }
        if(Hurler_map["flag9"]==0&&Hurler_map["flag8"]==1&&isclose("HurlerA",HurlerA_Pos,16.5,21.5)){
            move("hurler",16.5,22.5);
            Hurler_map["flag9"]=1;
            cerr<<"dddddd"<<endl;
        }

        //打印信息
        if(frame_count%50==0){
            cerr<<Hurler_map["flag6"]<<" "<<Hurler_map["flag7"]<<" "<<Hurler_map["flag8"]<<" "<<Hurler_map["flag9"]<<endl;
        }

    }
    
    if(GunnerA_HP>0){
        
        if(Gunner_map["flag1"]==0){
            move("gunner",22.5,4.5);
            Gunner_map["flag1"]=1;
        }
        if(Gunner_map["flag2"]==0){
            move("gunner",27.5,9.5);
            Gunner_map["flag2"]=1;
        }


    }





    // if(ismap==0){
    //     for(int i=0;i<game_map.size();i++){
    //         for(int j=0;j<game_map[i].size();j++){
    //             cerr<<game_map[i][j]<<" ";
    //         }
    //         cerr<<endl;
    //     }
    //     ismap=1;
    // }

    // cout << "skill hurler 1 10 10" << endl;
    // cout << "skill hurler 2" << endl;
    // cout << "skill gunner 1" << endl;
    // cout << "skill gunner 2" << endl;
    // cout << "skill medic 1 3.5 3.5" << endl;
    // cout << "skill medic 2 gunner" << endl;

    //不要删除这一行
    cout << "End" << endl;
}

int main()
{
    //初始化地图
    init_input();
    cerr << fixed << std::setprecision(4);
    while(true)
    {
        per_frame_input();
        output_command();
        cout<<"hello world"<<endl;
    } 
    return 0;
}