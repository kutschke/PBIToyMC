# PBIToyMC

  A toy MC to study what we can learn about proton bunch intensity
  variations within a spill using only the ExtMon.

  The input to the study is a time series of (t,pbi) values that
  cover one spill.  They came from a calculation reported by
  Steve Werkema in Mu2e-doc-33344.  There are two simulations
  each recorded as an .xlsx file in that document, one of a spill
  with a typical spill duty factor (SDF) and one with a pathlogically
  bad SDF.  The intial version of the study has two input files,
  which are extracted from the two xslx files.

  Normal.txt
  Pathological.txt

  The two columns are extracted from columns A and B in the xlsx files.
  See the file Spill.hh for a description of the meanings of the columns.

  To run the code,

# Setup a recent version of root.  See setup.sh for an example.
# root -l pbi.C

This will read the input file, run the toy MC, make some histograms
and ntuples.  It will draw some histograms to the screen and save
that screen as a pdf file.  It will save all histograms and ntuples
as a .root file.

