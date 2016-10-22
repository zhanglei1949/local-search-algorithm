#include<iostream>
#include<ctime>
#include<cstdlib>
#include<cstring>
#include<string>
#include<queue>
#include<set>
#include<map>
using namespace std;
// 4
// 3
// 2
// 1
//   1 2 3 4
// 判断是不是局部最小
class state
{
private:
    int col[9];// 八个位置，col[i]表示第i列的queen处在1-8的哪个位置
    int confliction[9]; //第icolumn的queen 的对手
    int confliction_total;
    //辅助数组
    //int m[9][9];
    int row[9];
    int diag_up[15];// diagonal goes up 0~14
    int diag_down[15]; //diagonal goes down

    int form()
    {
        for (int i = 1; i <= 8; ++i){
            row[i]=0;
            col[i]=0;
            confliction[i]=0;
            //for (int j = 1; j <= 8; ++j){
            //    m[i][j] = 0;
            //}
        }
        for (int i = 0; i <= 14; ++i){
            diag_down[i] = 0;
            diag_up[i] = 0;
        }

        for (int i = 1; i <= 8 ; ++i){
            col[i] = 1+8*rand()/RAND_MAX;
            //m[i][col[i]] = 1;
            row[col[i]] += 1;
            diag_up[i-col[i]+7]+=1;
            //cout<<"("<<i<<","<<col[i]<<"): diagonal "<<i-col[i]+7<<endl;
            diag_down[col[i]+i-2]+=1;
        }
        for (int i = 1; i <= 8; ++i){
            confliction[i] = row[col[i]] - 1 + diag_up[i-col[i]+7] - 1
                                + diag_down[col[i]+i-2] - 1;
        }
        confliction_total = 0;
        //cout<<"conflic[]"<<endl;
        for (int i = 1; i <= 8; ++i){
            //cout<<confliction[i]<<" ";
            confliction_total += confliction[i];
        }
        //cout<<endl;
        confliction_total = confliction_total / 2;
        //cout<<"confliction:"<<confliction_total<<endl;
//        for (int i = 1; i <= 8; ++i){
//            cout<<col[i]<<" ";
//        }
//        cout<<endl;
//        for (int i = 1; i <= 8; ++i){
//            cout<<row[i]<<" ";
//        }
//        cout<<endl;
//        cout<<"-----------------------"<<endl;
//
//        for (int j = 8; j >= 1; --j){
//            for (int i = 1; i <= 8; ++i){
//                cout<<m[i][j]<<" ";
//            }
//            cout<<endl;
//        }
//        cout<<"-----------------------"<<endl;;
    }
public:
    state()
    {
        form();
    }
    int reform()
    {
        //cout<<"reform"<<endl;
        form();
    }
    int get_queen_pos(int n)
    {
        return col[n];
    }
    int get_conf()
    {
        return confliction_total;
    }
    int cal_conf_after_move(int queen_col, int m)// move queen[i] to row m;
    {
        if (m == col[queen_col]){
            return confliction_total;
        }
        else {
            int res = 0;
            res = confliction_total - (row[col[queen_col]] - 1) + (row[m]) - (diag_up[queen_col-col[queen_col]+7] - 1)
                  + diag_up[queen_col - m + 7]  - (diag_down[queen_col + col[queen_col] - 2] - 1)
                  + diag_down[queen_col + m - 2];
            return res;
        }
    }
    int move(int column, int row_n)
    {
        //cout<<"move: "<<column<<" "<<row_n<<endl;
        row[col[column]]-=1;
        diag_up[column - col[column]+7]-=1;
        diag_down[column+col[column]-2]-=1;
        //m[column][col[column]]=0;
        //m[column][row_n] = 1;
        row[row_n]+=1;
        diag_up[column - row_n+7]+=1;
        diag_down[column+row_n-2]+=1;

        col[column] = row_n;

        for (int i = 1; i <= 8; ++i){
            confliction[i] = row[col[i]] - 1 + diag_up[i-col[i]+7] - 1
                                + diag_down[col[i]+i-2] - 1;
        }
        confliction_total = 0;
        for (int i = 1; i <= 8; ++i){
            confliction_total += confliction[i];
        }
        confliction_total = confliction_total / 2;
        //cout<<confliction_total<<endl;
    }
};
double simulate(int par_side_walk, int total_times)
{
    double res;
    int son_x[60]={0};
    int son_y[60]={0};
    int son_cnt = 0;
    srand(unsigned(time(0)));
    state s;
    int cnt = 0; // how many times have calculated
    //int total_times = 10000;
    int fail_times = 0;
    int success_times = 0;
    int conf_next[9][9] = {0};
    int current_conf = 0;
    int tmp_conf = 0;
    int tmp_min = 0;
    int what;
    int bool_judge = 0;
    //int par_side_walk = 40; // or 0 for no side walk
    while (cnt < total_times)
    {
        if (bool_judge > par_side_walk){
            fail_times++;
            cnt++;
            //cout<<"fail"<<endl;
            s.reform();
        }
        son_cnt = 0;
        //s_set.clear();
        tmp_min = s.cal_conf_after_move(1,1);
        current_conf = s.get_conf();
        //cout<<"current confliction: "<<current_conf<<endl;
        if (current_conf==0){
            //cout<<"success"<<endl;
            success_times++;
            s.reform();
            cnt++;
            continue;
        }

        // 父节点的冲突数目
        //calculate son nodes.
        for (int i = 1; i <= 8; ++i){
            // move queen at col [i]
            int queen_pos = s.get_queen_pos(i);
            for (int j = 1; j <= 8; ++j){
                if (j==queen_pos){
                    conf_next[i][j] = current_conf;
                    continue;
                }

                tmp_conf = s.cal_conf_after_move(i,j);
                conf_next[i][j] = tmp_conf;
                if (tmp_conf > current_conf) continue;
                if (tmp_conf < tmp_min){
                     tmp_min = tmp_conf;
                     son_cnt = 0;
                     //cout<<"m.clear()"<<endl;
                     son_x[son_cnt] = i;
                     son_y[son_cnt++] = j;
                     //cout<<"insert ("<<i<<","<<j<<") "<<conf_next[i][j]<<endl;

                }
                else if (tmp_conf == tmp_min){
                    son_x[son_cnt] = i;
                    son_y[son_cnt++] = j;
                    //cout<<"insert ("<<i<<","<<j<<") "<<conf_next[i][j]<<endl;
                }
            }
        }
        //cout<<"confliction map:"<<endl;
//        for (int i = 1; i <= 8; ++i){
//            for (int j = 1; j <= 8; ++j){
//                cout<<conf_next[i][j]<<" ";
//            }
//            cout<<endl;
//        }
        if (son_cnt == 0){
            //cout<<"size==0"<<endl;
            fail_times++;
            //cout<<"fail"<<endl;
            cnt++;
            s.reform();
        }
        else if (son_cnt == 1){
            //cout<<"size==1"<<endl;
            if (conf_next[son_x[0]][son_y[0]]==current_conf){
                        bool_judge++;
            }
            else bool_judge = 0;
            s.move(son_x[0], son_y[0]);
        }
        else {
            int choose = son_cnt*rand()/RAND_MAX;
            //cout<<"size=="<<son_cnt<<" choose "<<choose<<endl;
            for (int i = 0; i < son_cnt; ++i){
                if (i==choose){
                    if (conf_next[son_x[i]][son_y[i]]==current_conf){
                        bool_judge++;
                    }
                    else bool_judge = 0;
                    s.move(son_x[i], son_y[i]);
                    break;
                }
            }
        }
    }
    //cout<<fail_times<<" ";
    //cout<<success_times<<endl;
    res = double(fail_times)/total_times;
    //cout<<res<<endl;
    return res;
}
int main()
{
    double sum = 0;
    for (int i = 0; i < 10; ++i){
        sum += simulate(40,10000);
    }
    cout<<"With sideways move, failing rate is: "<<sum/10<<endl;
    sum = 0;
    for (int i = 0; i < 10; ++i){
        sum += simulate(0,10000);
    }
    cout<<"Without sideways move, the failing rate is: "<<sum/10<<endl;
}
