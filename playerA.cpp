#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>

using namespace std;

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

void output_command() //请不要删除End一行，每行输出记得加上换行符
{
    //示例代码
    if (frame_count < 1000)
    {
        cout << "move gunner 5 16.5" << endl;
    }else if (frame_count < 2000)
    {
        cout << "move gunner 15 16.5" << endl;
    }else
    {
        cout << "shoot gunner " << GunnerA_Pos.first + 1 << " " << GunnerA_Pos.second << endl;
    }
    cout << "skill hurler 1 10 10" << endl;
    cout << "skill hurler 2" << endl;
    cout << "skill gunner 1" << endl;
    cout << "skill gunner 2" << endl;
    cout << "skill medic 1 3.5 3.5" << endl;
    cout << "skill medic 2 gunner" << endl;

    //不要删除这一行
    cout << "End" << endl;
}

int main()
{
    //初始化地图
    init_input();

    while(true)
    {
        per_frame_input();
        output_command();
    } 
    return 0;
}