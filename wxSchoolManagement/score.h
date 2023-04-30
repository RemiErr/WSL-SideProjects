enum eSUBJECTS { CH_TW, ENG, MATH, PRO_I, PRO_II };
enum eEXAM { NONE, MIDTERM, FINAL };

class Score
{
private:
    int score;
    eSUBJECTS subj;
    eEXAM exam;

public:
    Score() { this->exam = NONE; }
    ~Score();
    void choose(eSUBJECTS subj, eEXAM exam) { this->subj = subj; this->exam = exam; }
    void setScore(int score);

    int getScore();
};