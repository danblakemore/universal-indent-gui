print "Help Desk -- What Editor do you use?";
chomp($editor = <STDIN>);
if ($editor =~ /emacs/i) {
  print "Why aren't you using vi?\n";
} elsif ($editor =~ /vi/i) {
  print "Why aren't you using emacs?\n";
} else {
  print "I think that's the problem\n";
}

{
  L9140:
 if ($msccom::obj==$msccom::food) {
 goto L8142;
 }
 if ($msccom::obj==$msccom::bird||$msccom::obj==$msccom::snake||$msccom::obj==$msccom::clam||$msccom::obj==$msccom::oyster||$msccom::obj==$msccom::dwarf||$msccom::obj==$msccom::dragon||$msccom::obj==$msccom::troll||$msccom::obj==$msccom::bear) {
 $msccom::spk=71;
 }
 goto L2011;
 # 
  #  DRINK.  IF NO OBJECT, ASSUME WATER AND LOOK FOR IT HERE.  IF WATER IS 
  #  THE BOTTLE, DRINK THAT, ELSE MUST BE AT A WATER LOC, SO DRINK STREAM. 
  # 
  L9150:
 if ($msccom::obj==0&&$liqloc->($placom::loc)!=$msccom::water&&($liq->(0)!=$msccom::water||!$here->($msccom::bottle))) {
 goto L8000;
 }
 if ($msccom::obj!=0&&$msccom::obj!=$msccom::water) {
 $msccom::spk=110;
 }
 if ($msccom::spk==110||$liq->(0)!=$msccom::water||!$here->($msccom::bottle)) {
 goto L2011;
 }
 $placom::prop->($msccom::bottle)=1;
 $placom::place->($msccom::water)=0;
 $msccom::spk=74;
 goto L2011;
 # 
  #  RUB.  YIELDS VARIOUS SNIDE REMARKS. 
  # 
  L9160:
 if ($msccom::obj!=$placom::lamp) {
 $msccom::spk=76;
 }
 goto L2011;
 # 
  #  THROW.  SAME AS DISCARD UNLESS AXE.  THEN SAME AS ATTACK EXCEPT IGNOR 
  #  AND IF DWARF IS PRESENT THEN ONE MIGHT BE KILLED.  (ONLY WAY TO DO SO 
  #  AXE ALSO SPECIAL FOR DRAGON, BEAR, AND TROLL.  TREASURES SPECIAL FOR 
  # 
  L9170:
 if ($toting->($msccom::rod2)&&$msccom::obj==$msccom::rod&&!$toting->($msccom::rod)) {
 $msccom::obj=$msccom::rod2;
 }
 }