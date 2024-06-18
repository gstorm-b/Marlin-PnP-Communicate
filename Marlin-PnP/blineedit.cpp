#include "blineedit.h"

BLineEdit::BLineEdit(QWidget* parent)
    : QLineEdit(parent) {

}

BLineEdit::~BLineEdit() {

}

void BLineEdit::saveText() {
  QString save_text = text();

  if ( saved_text.size() > 0) {
    QString last_text = saved_text.back();
    if (save_text == last_text) {
      return;
    }
  }

  saved_text.push_back(save_text);
  show_index = saved_text.size() - 1;

  if (saved_text.size() >= saved_limit) {
    saved_text.pop_front();
  }
}

void BLineEdit::changeSavedLimit(int limit) {
  saved_limit = limit;
}

int BLineEdit::getSavedLimit() {
  return saved_limit;
}

const QList<QString> BLineEdit::getSavedTextList() {
  return saved_text;
}

const QString BLineEdit::getSavedText(int index) {
  if (index == -1) {
    return saved_text.back();
  }

  if ( (index >= 0) && (index < (int)saved_text.size()) ) {
      return saved_text.at(index);
  }

  return "";
}

void BLineEdit::setTextFromSaved(int index) {
  setText(saved_text.at(index));
  show_index = index;
}

void BLineEdit::keyPressEvent(QKeyEvent *event) {

  int key = event->key();
  int temp_index;

  if (key == Qt::Key_Up) {
    temp_index = show_index - 1;
  } else if (key == Qt::Key_Down) {
    temp_index = show_index + 1;
  } else {
    QLineEdit::keyPressEvent(event);
    return;
  }

  if ( (temp_index < 0) || (temp_index >= saved_text.size()) ) {
    return;
  }

  setTextFromSaved(temp_index);
}

