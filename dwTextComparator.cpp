#include "dwTextComparator.h"
#include "diff_match_patch.h"
dwTextComparator::dwTextComparator(QTextEdit* textEdit1, QTextEdit* textEdit2, QString* text1, QString* text2)
    : textEdit1(textEdit1), textEdit2(textEdit2), text1(text1), text2(text2) {}

dwTextComparator::~dwTextComparator()
{

}

int dwTextComparator::compareText(int theme) {
    diff_match_patch dmp;
    QList<Diff> diffs = dmp.diff_main(*text1, *text2);
    dmp.diff_cleanupSemantic(diffs);
    QString highlightedText1;
    QString highlightedText2;

       int pos1 = 0;
       int pos2 = 0;
       int changesCounter = 0;
       QString colorGreen;
       QString colorRed;
       if (theme>-1)
       {
           if (theme)
           {
            colorRed = "rgb(109,59,59)";
            colorGreen = "rgb(59,109,61)";
           }
           else
           {
            colorRed = "rgb(255,153,153))";
            colorGreen = "rgb(204,255,204)";
           }
       }
       else
       {
           colorGreen="";
           colorRed="";
       }

       for (const auto& diff : diffs) {
           QString diffText = diff.text;

           if (diff.operation == Operation::DELETE) {

               highlightedText1 += QString ("<span style= \"background:%1\">%2</span>").arg(colorRed,diffText);
               pos1 += diffText.length();
           } else if (diff.operation == Operation::INSERT) {
               highlightedText2 += QString ("<span style= \"background:%1\">%2</span>").arg(colorGreen,diffText);
               pos2 += diffText.length();
               changesCounter++;
           } else if (diff.operation == Operation::EQUAL) {
               highlightedText1 += diffText;
               highlightedText2 += diffText;
               pos1 += diffText.length();
               pos2 += diffText.length();
           }
       }

       highlightedText1 += text1->mid(pos1);
       highlightedText2 += text2->mid(pos2);
       textEdit1->setText(highlightedText1);
       textEdit2->setText(highlightedText2);
    return changesCounter;
}
