/^# Packages using this file: / {
  s/# Packages using this file://
  ta
  :a
  s/ smalltalk / smalltalk /
  tb
  s/ $/ smalltalk /
  :b
  s/^/# Packages using this file:/
}
