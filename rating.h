#ifndef RATING_H
#define RATING_H

class Rating {
    int studentId;
    int score;
public:
    Rating(int sid, int s): studentId(sid), score(s) {}
    int getStudentId() const { return studentId; }
    int getScore() const { return score; }
};

#endif // RATING_H
