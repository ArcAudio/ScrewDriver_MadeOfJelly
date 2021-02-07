import("stdfaust.lib");
declare options "[midi:on][nvoices:12]";
string = hgroup("String[0]",+~(de.fdelay4(maxDelLength,delLength-1) : filter : * (damping) ) )*gate*gain
  with
{
 freq = hslider("[0]freq",440,50,5000,1);
 damping = hslider("[1]damping", 0.99,0,1,0.01);
 gain = hslider("[2]gain",1,0,1,0.01);
 gate = button("[3]gate");
 maxDelLength = 960;
 filter = _ <: _,_ ' :> / (2);
 delLength = ma.SR/freq;

};

process = string <: _,_;
			