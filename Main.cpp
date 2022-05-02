//Author : Srinivas Manda : IMT2020001 , Monjoy Narayan Choudhury : IMT2020502 
#include <bits/stdc++.h>
using namespace std;

int B = 10;
#define lli  int
lli precision = 0;
struct quorem
{
    lli Q;
    lli R;
};

quorem smalldiv(int tmp, int b)
{
    quorem p;
    if (tmp < 0)
    {
        if (tmp % b == 0)
        {
            p.Q = tmp / b;
            p.R = 0;
        }
        else
        {
            p.Q = -(abs(tmp) / B + 1);
            p.R = (abs(tmp) / B + 1) * (B)-abs(tmp);
        }
    }
    else
    {
        p.Q = tmp / B;
        p.R = tmp % B;
    }
    return p;
}

class Real_Number
{
public:
    int power;
    vector<lli> bits;

    Real_Number()
    {
        power = 0;
    }

    friend Real_Number operator+(Real_Number a, Real_Number b)
    {
        // Assume A>B ;
        vector<lli> ans;
        vector<lli> temp1;
        vector<lli> temp2;

        // Preprocessing for decimals. We have inherently assumed pow(a)>pow(b) so to
        // deal with the loop we take - , + accordingly
        lli noramlise_factor = a.power - b.power; // 0toappend
        for (int i = 0; i < -noramlise_factor; i++)
        {
            temp1.push_back(0);
        }
        for (int i = 0; i < a.bits.size(); i++)
        {
            temp1.push_back(a.bits[i]);
        }

        for (int i = 0; i < noramlise_factor; i++)
        {
            temp2.push_back(0);
        }

        for (int i = 0; i < b.bits.size(); i++)
        {
            temp2.push_back(b.bits[i]);
        }

        // storing new power for final decimal point location
        int decimal_position = max(a.power, b.power);
        // Now the Textbook algorithm starts
        lli a_size = temp1.size();
        lli b_size = temp2.size();
        lli intermediate_result;
        lli i = 0;
        lli j = 0;
        lli c = 0;
        lli s = 0;

        while (i < a_size && j < b_size)
        {
            intermediate_result = temp1[i++] + temp2[j++] + c;
            s = intermediate_result % B;
            if (s < intermediate_result)
                c = 1;
            else
                c = 0;
            ans.push_back(s);
        }
        // for remaining bits
        while (i < a_size)
        {
            intermediate_result = temp1[i++] + c;
            s = intermediate_result % B;
            if (s < intermediate_result)
                c = 1;
            else
                c = 0;
            ans.push_back(s);
        }

        while (j < b_size)
        {
            intermediate_result = temp2[j++] + c;
            s = intermediate_result % B;
            if (s < intermediate_result)
                c = 1;
            else
                c = 0;
            ans.push_back(s);
        }
        // if number of bits increase;
        if (c != 0)
        {
            ans.push_back(c);
        }

        Real_Number answer;
        answer.bits = ans;
        answer.power = decimal_position;
        return answer;
    }
    friend Real_Number operator-(Real_Number a, Real_Number b)
    {
        // Assume A>B ;
        vector<lli> ans;
        vector<lli> temp1;
        vector<lli> temp2;

        // Preprocessing for decimals. We have inherently assumed pow(a)>pow(b) so to
        // deal with the loop we take - , + accordingly
        lli noramlise_factor = a.power - b.power; // 0toappend
        for (int i = 0; i < -noramlise_factor; i++)
        {
            temp1.push_back(0);
        }
        for (int i = 0; i < a.bits.size(); i++)
        {
            temp1.push_back(a.bits[i]);
        }
        for (int i = 0; i < noramlise_factor; i++)
        {
            temp2.push_back(0);
        }

        for (int i = 0; i < b.bits.size(); i++)
        {
            temp2.push_back(b.bits[i]);
        }

        int decimal_position = max(a.power, b.power);
        // Now the Textbook algorithm starts
        lli a_size = temp1.size();
        lli b_size = temp2.size();
        lli intermediate_result;
        lli i = 0;
        lli j = 0;
        lli c = 0;
        lli d = 0;

        while (j < b_size)
        {
            intermediate_result = temp1[i] - temp2[j] + c;
            if (intermediate_result >= 0)
            {
                ans.push_back(intermediate_result);
                c = 0;
            }
            else
            {
                c = -1;
                ans.push_back(intermediate_result + B);
            }

            i++;
            j++;
        }

        while (i < a_size)
        {
            intermediate_result = temp1[i] + c;
            if (intermediate_result >= 0)
            {
                ans.push_back(intermediate_result);
                c = 0;
            }
            else
            {
                ans.push_back(B + intermediate_result);
                c = -1;
            }
            i++;
        }

        Real_Number answer;
        answer.bits = ans;
        answer.power = decimal_position;
        return answer;
    }
    friend Real_Number operator*(Real_Number a, Real_Number b)
    {
        // KARATSUBA : Note array stores number from LSB to MSB
        Real_Number prod;
        // Prepending zeros for Karatsuba
        while (a.bits.size() < b.bits.size())
            a.bits.push_back(0);
        while (b.bits.size() < a.bits.size())
            b.bits.push_back(0);

        // Base Case : size =1 ;

        if (a.bits.size() == 1)
        {
            int small_mul = a.bits[0] * b.bits[0]; // Here * is the normal multiplication
            if (small_mul / B != 0 || small_mul % B != 0)
            {
                prod.bits.push_back(small_mul % B);
            }
            if (small_mul / B != 0)
            {
                prod.bits.push_back(small_mul / B);
            }
        }

        // Divide and Conqeur Steps
        // Number Breakdown A = A0 | A1 (MSB...LSB)
        else
        {
            Real_Number A0, A1, B0, B1; // for spliting
            Real_Number x1, x2, x3;     // for intermediate calculation
            int a_size = a.bits.size(); // a==b so doesnt exactly matter now
            lli i = 0;
            // Do note the number is from LSB to MSB
            for (; i < a_size / 2; i++)
            {
                A1.bits.push_back(a.bits[i]);
                B1.bits.push_back(b.bits[i]);
            }
            for (; i < a_size; i++)
            {
                A0.bits.push_back(a.bits[i]);
                B0.bits.push_back(b.bits[i]);
            }

            for (i = 0; i < a_size / 2; i++)
            {
                x1.bits.push_back(0);
                x2.bits.push_back(0);
            }
            for (; i < 2 * (a_size / 2); i++)
            {
                x1.bits.push_back(0);
            }

            Real_Number p1, p2, p3, p4, p5; // To hold products of the fragments
            // Recursive Calls
            p1 = A0 * B0;
            p2 = A1 * B1;
            // Storing this data in accumulator with proper offset (done in above loops)
            x1.bits.insert(x1.bits.end(), p1.bits.begin(), p1.bits.end());
            x3 = p2;
            // Conquer Step
            p4 = A0 + A1;
            p5 = B0 + B1;

            p3 = p4 * p5 - p1 - p2;
            x2.bits.insert(x2.bits.end(), p3.bits.begin(), p3.bits.end());
            prod = x1 + x2 + x3;
        }
        while (prod.bits.size() > 0 and prod.bits[prod.bits.size() - 1] == 0)
        {
            // removing prepending 0s : avoiding numbers like 0000000011111 and fixing it to 11111...
            prod.bits.pop_back();
        }
        prod.power = a.power + b.power;
        return prod;
    }
    friend pair<Real_Number, Real_Number> operator/(Real_Number a, Real_Number b)
    {
        // Algortihm referred as in shoups ;
        Real_Number temp_a;
        temp_a.power = 0;
        int temp;
        // Pre-processing
        if (!(a.power > precision + b.bits.size() + 3))
        {
            for (int i = 0; i < precision + b.bits.size() - a.power + 3; i++)
            {
                temp_a.bits.push_back(0);
                temp_a.power++;
            }
            temp = 0;
        }
        else
        {
            temp = precision + b.bits.size() - a.power + 3;
            temp = -temp;
        }
        for (int i = temp; i < a.bits.size(); i++)
        {
            temp_a.bits.push_back(a.bits[i]);
            if (i < a.power)
                temp_a.power++;
        }

        Real_Number temp_b;
        lli j = 0;
        for (j = b.bits.size() - 1;; j--)
        {
            if (b.bits[j] != 0)
                break;
        }

        for (; j >= 0; j--)
        {
            temp_b.bits.push_back(b.bits[j]);
        }
        reverse(temp_b.bits.begin(), temp_b.bits.end());
        temp_b.power = b.power;
        Real_Number Quo, Rem; // q->Quo r-> Rem
        lli a_size = temp_a.bits.size();
        lli b_size = temp_b.bits.size();
        // if a<b return q = 0 ; r= a ;
        if (a_size < b_size)
        {
            pair<Real_Number, Real_Number> result;
            Quo.bits.push_back(0);
            result.first = Quo;
            result.second = a;
            return result;
        }

        for (int i = 0; i < a_size; i++)
        {
            Rem.bits.push_back(temp_a.bits[i]);
        }
        Rem.bits.push_back(0);

        for (int i = 0; i <= a_size - b_size; i++)
        {
            Quo.bits.push_back(0);
        }

        for (int i = a_size - b_size; i >= 0; i--)
        {
            Quo.bits[i] = (Rem.bits[i + b_size] * B + Rem.bits[i + b_size - 1]) / temp_b.bits[b_size - 1];
            if (Quo.bits[i] >= B)
            {
                Quo.bits[i] = B - 1;
            }
            int carry = 0, tmp;
            for (int j = 0; j < b_size; j++)
            {
                tmp = Rem.bits[i + j] - Quo.bits[i] * temp_b.bits[j] + carry;
                quorem qr = smalldiv(tmp, B);
                Rem.bits[i + j] = qr.R;
                carry = qr.Q;
            }
            Rem.bits[i + b_size] += carry;
            while (Rem.bits[i + b_size] < 0)
            {
                carry = 0;
                for (int j = 0; j < b_size; j++)
                {
                    tmp = Rem.bits[i + j] + temp_b.bits[j] + carry;
                    quorem qr = smalldiv(tmp, B);
                    Rem.bits[i + j] = qr.R;
                    carry = qr.Q;
                }
                Rem.bits[i + b_size] += carry;
                Quo.bits[i]--;
            }
        }

        int end = Quo.bits.size() - 1;
        while (Quo.bits[end--] == 0 && end >= 0)
            Quo.bits.pop_back();
        if (Quo.bits.size() == 0)
            Quo.bits.push_back(0);

        Quo.power = temp_a.power - b.power;

        if (Quo.bits[0] > B / 2)
            Quo.bits[2]++;
        else if (Quo.bits[0] == B / 2)
        {
            if (Quo.bits[1] > B / 2 || (Quo.bits[1] == B / 2 && Quo.bits[2] % 2))
                Quo.bits[2]++;
        }
        Quo.bits[0] = 0;
        Quo.bits[1] = 0;

        pair<Real_Number, Real_Number> answer;
        answer.first = Quo;
        answer.second = Rem;
        return answer;
    }
};

Real_Number Newton_Rapson(Real_Number a)
{
    // Algorithm as per PDF ;
    // For the /2 we need to convert int 2 to our type
    Real_Number two;
    two.power = 0;
    two.bits.push_back(2);

    Real_Number x0;
    x0.power = 1;
    // Just a randomly picked X0
    x0.bits.push_back(5);
    x0.bits.push_back(1); 
   
    for (lli i = 0; i < 8 * log(precision); i++)
    {
        x0 = ((x0 + (a / x0).first) / two).first;
    }
    return x0;
}

Real_Number Evaluate_PI()
{

    // As per PDF algorithm ;
    // Setting base values;
    Real_Number two;
    two.power = 0;
    two.bits.push_back(2);

    Real_Number an, bn, pn;
    Real_Number an_1, bn_1, pn_1;

    an = Newton_Rapson(two);

    bn.power = 0;
    bn.bits.push_back(0);
    pn = two + an;

    Real_Number one;
    one.power = 0;
    one.bits.push_back(1);
    // Initialising temp values to compute intermediate values in algorithm

    Real_Number t1, t2, t3, t4;

    for (lli i = 0; i < 8 * log(precision); i++)
    {

        an_1 = ((Newton_Rapson(an) + (one / Newton_Rapson(an)).first) / two).first;
        bn_1 = (((one + bn) * Newton_Rapson(an)) / (an + bn)).first;

        t1 = one + an_1;
        t2 = t1 * pn;
        t3 = t2 * bn_1;
        t4 = one + bn_1;
        pn_1 = (t3 / t4).first;
        an = an_1;
        bn = bn_1;
        pn = pn_1;
    }
    return pn;
}
int main()
{
    Real_Number n;
    n.power = 0;
    n.bits.push_back(2);
    lli choice;

    lli m;
    cin >> choice >> precision >> m;
    if (choice == 1)
    {
        Real_Number PI = Evaluate_PI();
        for (lli i = PI.bits.size() - 1; i >= PI.bits.size() - precision - 1; i--)
        {
            if (i == PI.power - 1)
                cout << ".";
            cout << PI.bits[i];
        }
        cout << endl;
    }
    else if (choice == 2)
    {
        Real_Number a = Newton_Rapson(n);
        for (lli i = a.bits.size() - 1; i >= a.bits.size() - precision - 1; i--)
        {
            if (i == a.power - 1)
                cout << ".";
            cout << a.bits[i];
        }

        cout << endl;
    }

    return 0;
}