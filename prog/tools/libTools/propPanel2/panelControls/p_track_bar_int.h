#pragma once

#include "p_base.h"
#include "../windowControls/w_track.h"
#include "../windowControls/w_spin_edit.h"
#include <propPanel2/c_panel_base.h>


class CTrackBarInt : public BasicPropertyControl
{
public:
  CTrackBarInt(ControlEventHandler *event_handler, PropertyContainerControlBase *parent, int id, int x, int y, hdpi::Px w,
    const char caption[], int min, int max, int step);

  static PropertyContainerControlBase *createDefault(int id, PropertyContainerControlBase *parent, const char caption[],
    bool new_line = true);

  unsigned getTypeMaskForSet() const { return CONTROL_DATA_TYPE_INT | CONTROL_DATA_MIN_MAX_STEP | CONTROL_CAPTION; }
  unsigned getTypeMaskForGet() const { return CONTROL_DATA_TYPE_INT; }

  int getIntValue() const;
  void setIntValue(int value);
  void setMinMaxStepValue(float min, float max, float step);
  void setCaptionValue(const char value[]);

  virtual void reset();

  void setEnabled(bool enabled);
  void setWidth(hdpi::Px w);
  void moveTo(int x, int y);

protected:
  virtual void onWcChange(WindowBase *source);

private:
  WStaticText mCaption;
  WTrackBar mTrackBar;
  WSpinEdit mEditor;
  int mValue;
};
