#define FLOOR 1
#define FLOOR_VCX 3
//ÿ�θ���FLOOR����Ҫ��board.c�е�priority, best_score_of_upper, not_in_the_same_branch����������Ĵ�С��һ��
#define Consecutive_Five 10000000//����
#define Open_Four 1000000//����
#define Open_Three 100000//����
#define Gapped_Two_Two 100050//�������ֶ��ǳ��ģ�����������2��֮��ȱ��һ����  ��񣿡��
#define Capped_Four 100050//���ֳ�����һͷ����ס�ĳ���   ������͡�����
#define Gapped_Four 100050//���ֳ�����һ��������һ����������֮�����һ��  ��?����
#define Gapped_Three 99950//���������������Ǽ����γɻ��ĵ���  �񣿡��
#define Capped_Three 10000//����, ���ֻ���γɳ���  �����
//��Ҫ�����Ĵ���� ���1000��
/*����һ�����⼸����̬��
1. �����__
2. ����_��_
3. ���_���_
4. ���__��
5. ��_��_��
6. ��_����_��
*/
//���潫��ͬ��������һ��
#define Capped_Two_one 9000//����_��_������
#define Capped_One_Two 9500//���_���_
#define Gapped_Two_one 7000//���__��
#define Gapped_One_One_One 6000//��_��_��
#define Capped_Capped_Three 5000//

#define Open_two 1000//����
#define depth_of_hashing 100000
void DrawBoard(char board[][17][3], int i, long int value, int mode_choice, int coordinate[], int step_count);
void chess_play(char **board, int step_count);
bool judgement(char board[][17][3], int step_count);
long int evaluation(char board[][17][3], int step_count, bool my_turn, int raw, int column);
void get_coordinate(int coordinate[], char board[][17][3], int step_count);
void chess_play_ver2(char board[][17][3], int step_count, int coordinate[]);
long int Minimax2(char board[][17][3], int step_count, bool my_turn, bool ai_first, int floor, int coordinate[], long int best_score_of_upper[], int priority[][30][2], bool not_in_the_same_branch[], unsigned long long hashValue, unsigned long long ZobristTable[15][15][2], unsigned long long hashing_value2[depth_of_hashing][3], int fatal_priority[][32][2], long int fatal_best_score_of_upper[], bool fatal_not_in_the_same_branch[]);
void auto_play(char board[][17][3], char chess[], char opponent_chess[], int coordinate[]);
bool verify_coordinate(char board[][17][3], int raw, int column, char chess, char opponent_chess);
bool before_evaluation(char board[][17][3], int priority[][30][2], int floor, int step_count, bool my_turn);
void return_to_normal_chess(char board[][17][3], int step_count, int coordinate[]);
void initialize_hashing_sheet(long long int key[][15][2]);
long int Zobrist_hashing(long long int hashing_value[][3], long long int key[][15][2], int best_raw, int best_column, bool write_new_coordinate, int step_count, char board[][17][3], bool my_turn, long long int hashing_value_now);
bool find_fatal_point(char board[][17][3], int fatal_priority[][32][2], int floor_vcx, int step_count, bool my_turn);
long int fatal_step(char board[][17][3], int step_count, bool my_turn, bool ai_first, int floor_vcx, int coordinate[], long int fatal_best_score_of_upper[], int fatal_priority[][32][2], bool fatal_not_in_the_same_branch[]);
void init_genrand64(unsigned long long seed);
unsigned long long genrand64_int64(void);
int indexOf(char chess[3]);
void initTable(unsigned long long ZobristTable[15][15][2]);
unsigned long long computeHash(char board2[15][17][3], unsigned long long ZobristTable[15][15][2]);
long int Searching_Hashing(unsigned long long hashing_value2[][3], unsigned long long ZobristTable[15][15][2], int step_count, unsigned long long hashValue, bool my_turn, long temp_score, bool write);
void init_genrand64(unsigned long long seed);
int before_evaluation_ver2(char board[][17][3], int priority[][30][2], int floor, int step_count, bool my_turn);
void quick_sort(int temp_priority[][3], int l, int r);
int before_evaluation_ver3(char board[][17][3], int priority_ver2[][2], int floor, int step_count, bool my_turn, char **chess_pointer);