#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>
#include <map>
#include <iomanip>
#include <unordered_map>
#include <queue>
#include <algorithm>
using namespace std;
#define NOT_FIND ="not find"

int Gunner_frame;
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
float blood[3];
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

int medic1_flag=0;
pair<float,float> Gunner_Last_Pos{0.0f, 0.0f};//记录上一次的位置
pair<float,float> Hurler_Last_Pos{0.0f, 0.0f};
pair<float,float> Medic_Last_Pos{0.0f, 0.0f};
bool holdGunner=false;
bool holdHurler=false;
bool holdMedic=false;

vector<Fire> fire_pos; //场上火焰的位置
vector<Potion> potion_pos; //场上药水的位置
int frame_count; //帧数信息

struct Enemy {
    string name;            // 敌人名称
    float HP;               // 敌人血量
    pair<float, float> Pos; // 敌人位置 (x, y)

    // 构造函数（带位置参数）
    Enemy(const string& n, float h, pair<float, float> p) : name(n), HP(h), Pos(p) {}

    // 默认构造函数（返回默认敌人）
    Enemy() : name("notfind"), HP(-1.0f), Pos({0.0f, 0.0f}) {}

    // 重载小于运算符（用于优先队列，血量小的优先级更高）
    bool operator<(const Enemy& other) const {
        return HP > other.HP; // 小根堆，血量越小优先级越高
    }
};
Enemy Gunner_enemy("notfind",-1.0f,{0.0f,0.0f});
Enemy Hurler_enemy("notfind",-1.0f,{0.0f,0.0f});
Enemy Medic_enemy("notfind",-1.0f,{0.0f,0.0f});

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

bool is_wall_between(pair<float, float> start, pair<float, float> end) {
    int x1 = static_cast<int>(start.first);
    int y1 = static_cast<int>(start.second);
    int x2 = static_cast<int>(end.first);
    int y2 = static_cast<int>(end.second);

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (game_map[y1][x1] == 2) {
            return true; // 有墙体阻隔
        }
        if (x1 == x2 && y1 == y2) {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    return false; // 没有墙体阻隔
}

bool isAroud(pair<float,float> pos1,pair<float,float> pos2){    //判断pos1是否在pos2附近
    if(abs(pos1.first-pos2.first)<=20&&abs(pos1.second-pos2.second)<=20){
        return true;
    }
    return false;
}

bool isVisible(pair<float, float> targetPos) {
    return !(targetPos.first == 0 && targetPos.second == 0);
}

float calculateDistance(pair<float, float> pos1, pair<float, float> pos2) {
    return sqrt(pow(pos1.first - pos2.first, 2) + pow(pos1.second - pos2.second, 2));
}

Enemy find_Gunner_enemy(){
    priority_queue<Enemy> enemyQueue;

    //GunnerA的检测
    if(isVisible(GunnerA_Pos)&&isAroud(GunnerA_Pos,GunnerB_Pos)){
        // cerr<<"GunnerA_HP="<<GunnerA_HP<<endl;
        // cerr<<"GunnerA_Pos="<<GunnerA_Pos.first<<" "<<GunnerA_Pos.second<<endl;
        enemyQueue.push(Enemy("GunnerA",GunnerA_HP,GunnerA_Pos));
        if(enemyQueue.top().HP<=5) enemyQueue.pop();
    }
    
    //HurlerA的检测
    if(isVisible(HurlerA_Pos)&&isAroud(HurlerA_Pos,GunnerB_Pos)){
        // cerr<<"HurlerA_HP="<<HurlerA_HP<<endl;
        // cerr<<"HurlerA_Pos="<<HurlerA_Pos.first<<" "<<HurlerA_Pos.second<<endl;
        enemyQueue.push(Enemy("HurlerA",HurlerA_HP,HurlerA_Pos));
        if(enemyQueue.top().HP<=5) enemyQueue.pop();
    }

    //MedicA的检测
    if(isVisible(MedicA_Pos)&&isAroud(MedicA_Pos,GunnerB_Pos)){
        // cerr<<"MedicA_HP="<<MedicA_HP<<endl;
        // cerr<<"MedicA_Pos="<<MedicA_Pos.first<<" "<<MedicA_Pos.second<<endl;
        enemyQueue.push(Enemy("MedicA",MedicA_HP,MedicA_Pos));
        if(enemyQueue.top().HP<=5) enemyQueue.pop();
    }
    // cerr<<"enemyQueue.size()="<<enemyQueue.size()<<endl;
    if(enemyQueue.empty()){
        return Enemy();
    }
    else{
        return enemyQueue.top();
    }
}

void shoot(string name, float pos_x, float pos_y){
    // cout<<"shoot Gunner "<<Gunner_enemy.Pos.first<<" "<<Gunner_enemy.Pos.second<<endl;
    cout<<"shoot"<<" "<<name<<" "<<pos_x<<" "<<pos_y<<endl;
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

pair<float,float> find_lowest_pos(int i){//寻找血量最低朋友的位置
    if(i==0){
        return GunnerB_Pos;
    }
    else if(i==1){
        return HurlerB_Pos;
    }
    else if(i==2){
        return MedicB_Pos;
    }
    else{
        return make_pair(0,0);
    }
}
string find_lowest_name(int i){//找到血量最低的朋友名称
    if(i==0){
        return "Gunner";
    }
    else if(i==1){
        return "Hurler";
    }
    else if(i==2){
        return "Medic";
    }
    else{
        return " ";
    }

}
string find_lowest_blood(void){    //找到name的朋友
    float lowest_friend=1000;
    pair<float,float> lowest_friend_pos;
    string lowest_friend_name;
    for(int i = 0; i < 3; i++){
        if(blood[i]<lowest_friend){
            lowest_friend=blood[i];
            lowest_friend_pos=find_lowest_pos(i);
            lowest_friend_name=find_lowest_name(i);
        }
    }
    return lowest_friend_name;
}
void cure_friends(void){    //治疗朋友
    if(frame_count<=100){
        cout<<"skill Medic 2 Hurler"<<endl;
        // cerr<<"skill MedicA 2 HurlerA"<<endl;
    }
    string friend_name = find_lowest_blood();
    // cerr << "Lowest blood friend: " << friend_name << endl;
    if(frame_count%3000==1||frame_count%3000==2){
        cout << "skill "<<"Medic"<<" 2 "<< friend_name << endl;
        // cerr << "Executing skill on: " << friend_name << endl;
    }
    return;
}

void Gunner_patrol(){

    if(Gunner_map["flag6"]==0&&isclose("GunnerB",GunnerB_Pos,12.5,28.5)){
        // cerr<<"move to 27.5 24.5 patrol"<<endl;
        move("Gunner",12.5,15.5);
        Gunner_map["flag6"]=1;
        Gunner_map["flag5"]=0;
    }
    if(Gunner_map["flag5"]==0&&isclose("GunnerB",GunnerB_Pos,12.5,15.5)){
        // cerr<<"move to 27.5 11.5 patrol"<<endl;
        move("Gunner",12.5,28.5);
        Gunner_map["flag5"]=1;
        Gunner_map["flag6"]=0;
    } 
}

void medic_skill1(pair<float,float>pos){ //medic释放技能1
    cout<<"skill Medic 1 "<<pos.first<<" "<<pos.second<<endl;
}

bool ishold(string name,pair<float,float> pos){   
    if(name=="GunnerB"){
        if(abs(Gunner_Last_Pos.first-pos.first)<=1&&abs(Gunner_Last_Pos.second-pos.second)<=1){
            Gunner_Last_Pos=pos;
            return true;
        }
        else{
            Gunner_Last_Pos=pos;
            return false;
        }
    }
    else if(name=="HurlerB"){
        if(abs(Hurler_Last_Pos.first-pos.first)<=1&&abs(Hurler_Last_Pos.second-pos.second)<=1){
            Hurler_Last_Pos=pos;
            return true;
        }
        else{
            Hurler_Last_Pos=pos;
            return false;
        }
    }
    else if(name=="MedicB"){
        if(abs(Medic_Last_Pos.first-pos.first)<=1&&abs(Medic_Last_Pos.second-pos.second)<=1){
            Medic_Last_Pos=pos;
            return true;
        }
        else{
            Medic_Last_Pos=pos;
            return false;
        }
    }
    return false;
}

void print_pos(string name){
    if (name=="GunnerB")
    {
        cerr<<"GunnerB_Pos:"<<GunnerB_Pos.first<<" "<<GunnerB_Pos.second<<endl;
    }else if(name=="HurlerB"){
        cerr<<"HurlerB_Pos:"<<HurlerB_Pos.first<<" "<<HurlerB_Pos.second<<endl;
    }else if(name=="MedicB"){
        cerr<<"MedicB_Pos:"<<MedicB_Pos.first<<" "<<MedicB_Pos.second<<endl;
    }
    return ;
}










void output_command() //请不要删除End一行，每行输出记得加上换行符
{
    if(MedicB_HP>0){
        if(frame_count%50==0){
            blood[0]=GunnerB_HP;
            blood[1]=HurlerB_HP;
            blood[2]=MedicB_HP;
            holdMedic=ishold("MedicB",MedicB_Pos);
            // ishold("GunnerB",GunnerB_Pos);
            // ishold("HurlerB",HurlerB_Pos);
        }
        if(frame_count%50==1||frame_count%50==2){
            cure_friends();
            if(holdMedic==true){
                // cerr<<"hold medic"<<endl;
            }
        }
        if(frame_count>50&&Medic_map["flag0"]==0){
            move("medic",33.5,23.5);
            Medic_map["flag0"]=1;
        }
        if(Medic_map["flag1"]==0&&isclose("MedicB",MedicB_Pos,33.5,23.5)){
            move("medic",23.5,23.5);
            Medic_map["flag1"]=1;
        }
        if(Gunner_map["flag3"]==1&&(isclose("GunnerB",GunnerB_Pos,12.5,15.5)||holdGunner==true)){
            if(medic1_flag==0){
                medic_skill1(GunnerB_Pos);
                medic1_flag=frame_count;
                // cerr<<"medic skill1"<<endl;
            }
            else{
                if(frame_count-medic1_flag>=1250){
                    medic_skill1(GunnerB_Pos);
                    medic1_flag=frame_count;
                }
            }
        }
        //打印medic的位置
        if((frame_count%100)==0){
            print_pos("MedicB");
        }
    }

    if(HurlerB_HP>0){

        if(frame_count%50==0){
            holdHurler=ishold("HurlerB",HurlerB_Pos);
            // print_pos("HurlerB");
        }

        //hurler释放技能1
        cout<<"skill hurler 1 11.5 13.5"<<endl;

        
    
        //hurler移动到4.5,16.5
        if(Hurler_map["flag1"]==0){
            cerr<<"move to 33.5 23.5"<<endl;
            move("hurler",35.5,23.5);
            Hurler_map["flag1"]=1;
        }
        
        //hurler移动到12,21.5
        if(Hurler_map["flag2"]==0&&isclose("HurlerB",HurlerB_Pos,35.5,23.5)){
            cerr<<"move to 28 18.5"<<endl;
            move ("hurler",24.5,23.5);
            Hurler_map["flag2"]=1;
        }

        //打印hurler的位置
        if((frame_count%100)==0){
            print_pos("HurlerB");
        }

        //

        //hurler释放技能
        if(Hurler_map["flag3"]==0&&isclose("HurlerB",HurlerB_Pos,28,18.5)){
            // cout<<"skill hurler 1 38 38"<<endl;
            move("hurler",28,17.5);
            Hurler_map["flag3"]=1;
        }

        //hurler移动到16.5,22.5
        if(Hurler_map["flag4"]==0&&isclose("HurlerB",HurlerB_Pos,28,17.5)){
            move("hurler",23.5,17.5);
            Hurler_map["flag4"]=1;
        }

        //hurler释放技能到26.5，25.5
        // if(Hurler_map["flag5"]==0&&isclose("HurlerB",HurlerB_Pos,16.5,22.5)){
        //     cout<<"skill hurler 1 26.5 25.5"<<endl;
        //     Hurler_map["flag5"]=1;
        // }

        //刷新hurler技能
        // if(frame_count%750==0){
        //     Hurler_map["flag5"]=0;
        // }

        //hurler去找金币
        if(has_coin&&Hurler_map["flag6"]==0&&isclose("HurlerB",HurlerB_Pos,23.5,17.5)){
            move("hurler",23.5,18.5);
            Hurler_map["flag6"]=1;
            // cerr<<"aaaaaa"<<endl;
        }

        if(has_coin&&Hurler_map["flag7"]==0&&(isclose("HurlerB",HurlerB_Pos,23.5,18.5)||isclose("HurlerB",HurlerB_Pos,24.5,23.5))){
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
            //hurler释放技能2
            cout<<"skill hurler 2"<<endl;
            move("hurler",23.5,18.5);
            // cerr<<"cccccc"<<endl;
            Hurler_map["flag8"]=1;
        }
        if(Hurler_map["flag9"]==0&&Hurler_map["flag8"]==1&&isclose("HurlerB",HurlerB_Pos,23.5,18.5)){
            move("hurler",23.5,17.5);
            Hurler_map["flag9"]=1;
            // cerr<<"dddddd"<<endl;
        }

        //打印信息
        if(frame_count%50==0){
            // cerr<<Hurler_map["flag6"]<<" "<<Hurler_map["flag7"]<<" "<<Hurler_map["flag8"]<<" "<<Hurler_map["flag9"]<<endl;
        }

    }

        if(GunnerB_HP>0){
        
        if(frame_count%50==0){
            holdGunner=ishold("GunnerB",GunnerB_Pos);
        }

        //Gunner释放技能1
        if(frame_count%1000==1){
            cout<<"skill Gunner 1"<<endl;
        }

        if(frame_count%100==0){
            Gunner_enemy= find_Gunner_enemy();
            cerr<<"isfind_Gunner_enemy name:"<<Gunner_enemy.name<<endl;
            bool has_wall = is_wall_between(GunnerB_Pos, Gunner_enemy.Pos);
            if (has_wall) {
                cerr << "yes there is a wall" << endl;
            } else {
                cerr << "no there is not a wall" << endl;
            }
            }
        
        if(Gunner_enemy.name!="notfind"&&is_wall_between(GunnerB_Pos, Gunner_enemy.Pos)==false){
            Gunner_map["stage_init"]=1;
            // cerr<<"enemy name:"<<Gunner_enemy.name<<endl;
            // cerr<<"enemy pos:"<<Gunner_enemy.Pos.first<<" "<<Gunner_enemy.Pos.second<<endl;
            cout<<"skill Gunner 2"<<endl;
            shoot("Gunner",Gunner_enemy.Pos.first,Gunner_enemy.Pos.second);
            Gunner_map["stage_fire"]=1;
            // cout<<"shoot Gunner "<<Gunner_enemy.Pos.first<<" "<<Gunner_enemy.Pos.second<<endl;
            // cout<<"shoot Gunner 1 30 "
            Gunner_map["stage_back"]=0;
        }else if((Gunner_enemy.name=="notfind"&&Gunner_map["stage_fire"]==1)||(is_wall_between(GunnerB_Pos, Gunner_enemy.Pos)==true&&Gunner_enemy.name!="notfind")){
            // cerr<<"not find enemy"<<endl;
            Gunner_map["stage_fire"]=0;
            Gunner_map["stage_unfire"]=1;
            Gunner_frame=frame_count;
        }

        if(frame_count>=Gunner_frame+20&&Gunner_map["stage_init"]==1&&Gunner_map["stage_fire"]==0
        &&Gunner_map["stage_unfire"]==1&&Gunner_map["stage_back"]==0){
            // cerr<<"move to 27.5 11.5 aaaaaaaaaaaaaa"<<endl;
            move("Gunner",12.5,28.5);
            Gunner_map["stage_back"]=1;
        }

        if(Gunner_map["stage_back"]==1){
            Gunner_patrol();   
        }

        // //不打架就巡逻
        // if(holdGunner==true&&Gunner_map["stage_fire"]==0&&Gunner_map["fresh_Gunner_Pos"]==0){
        //     // cerr<<"Gunner is hold start to patrol"<<endl;
        //     cerr<<"fresh_Gunner_Pos 333333333"<<endl;
        //     fresh_Gunner_Pos();
        //     // cout<<"move Gunner 27.5 11.5"<<endl;
        //     cerr<<"in patrol GunnerB_Pos:"<<GunnerB_Pos.first<<" "<<GunnerB_Pos.second<<endl;
        //     cerr<<"fresh_Gunner_Pos11111111"<<endl;
        //     Gunner_map["fresh_Gunner_Pos"]=1;
        // }


        // if(Gunner_map["fresh_Gunner_Pos"]==1&&isclose("GunnerB",GunnerB_Pos,27.5,11.5)){
        //     cerr<<"fresh_Gunner_Pos22222222"<<endl;
        //     Gunner_patrol();
        // }



        if(Gunner_map["stage_init"]==0){
            if(Gunner_map["flag0"]==0){
            move("gunner",33.5,35.5);
            Gunner_map["flag0"]=1;
            }

            if(Gunner_map["flag1"]==0&&isclose("GunnerB",GunnerB_Pos,33.5,35.5)){
                move("gunner",18.5,35.5);
                // cerr<<"gunner move to 21.5,4.5"<<endl;
            
                Gunner_map["flag1"]=1;
            }


            if(Gunner_map["flag2"]==0&&isclose("GunnerB",GunnerB_Pos,18.5,35.5)){
                move("gunner",12.3,30.5);
                Gunner_map["flag2"]=1;
            }

            //Gunner去27.5,11.5
            if(Gunner_map["flag3"]==0&&isclose("GunnerB",GunnerB_Pos,12.3,30.5)){
                move("gunner",12.3,28.5);
                Gunner_map["flag3"]=1;
            }

            //Gunner去27.5，24.5
            if(Gunner_map["flag4"]==0&&isclose("GunnerB",GunnerB_Pos,12.3,28.5)){
                move("gunner",12.3,15.5);
                Gunner_map["flag4"]=1;
            }
        }
    }



   
    //不要删除这一行
    cout << "End" << endl;
}

int main()
{
    //初始化地图
    init_input();
    game_map[16][22]=2;
    game_map[25][19]=2;
    cerr << fixed << std::setprecision(4);
    while(true)
    {
        per_frame_input();
        output_command();
    } 
    return 0;
}