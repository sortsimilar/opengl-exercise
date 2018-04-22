#include<algorithm>
#include<iostream>
#include<string>
#include<vector>
using namespace std;

class Testnee
{
public:
	string reg;
	int score;
	int final_rank;
	int local_rank;
	int location;
};


bool compare(Testnee a, Testnee b)
{
	if (a.score == b.score)
		return a.reg < b.reg;
	else
		return a.score > b.score;
}


int main()
{
	vector<Testnee> all_testnee;

	int N; // num of test locations;
	cin >> N;

	for (int i = 0;i < N;i++)
	{
		int K;
		cin >> K;

		vector<Testnee> local_testnee(K);
		for (int j = 0;j < K;j++)
		{
			
			cin >> local_testnee[j].reg;
			cin >> local_testnee[j].score;
			local_testnee[j].location = i + 1;
		}
		
		sort(local_testnee.begin(), local_testnee.end(), compare);

		local_testnee[0].local_rank = 1;
		for (int j = 1;j < K;j++)
		{
			if (local_testnee[j].score == local_testnee[j - 1].score)
			{
				local_testnee[j].local_rank = local_testnee[j - 1].local_rank;
			}
			else
			{
				local_testnee[j].local_rank = j + 1;
			}
		}

		/*
		// test local rank result;
		for (int j = 0;j < K;j++)
		{
			cout << local_testnee[j].reg << " " << local_testnee[j].score <<" " << local_testnee[j].local_rank << endl;
		}*/

		for (int j = 0;j < K;j++)
		{
			all_testnee.push_back(local_testnee[j]);
		}

	}


	sort(all_testnee.begin(), all_testnee.end(), compare);
	
	all_testnee[0].final_rank = 1;
	for (int i = 1;i < all_testnee.size();i++)
	{
		if (all_testnee[i].score == all_testnee[i - 1].score)
			all_testnee[i].final_rank = all_testnee[i - 1].final_rank;
		else
			all_testnee[i].final_rank = i + 1;
	}

	// show result;
	cout << all_testnee.size();
	if (all_testnee.size() > 0) cout << endl;

	for (int i = 0;i < all_testnee.size();i++)
	{
		cout << all_testnee[i].reg<<" "<<all_testnee[i].final_rank<<" ";
		cout << all_testnee[i].location << " " << all_testnee[i].local_rank;

		if (i != all_testnee.size() - 1) cout << endl;
	}



	system("pause");
	return 0;
}