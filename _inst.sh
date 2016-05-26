#!/bin/bash --
#  (call bash, but do not allow additional CLI args -- cracker resistent)

###-------------------------------------------------------------------
###-initialize--------------------------------------------------------
###-------------------------------------------------------------------


#---(utility constants)-----------------------#
declare   PRINTF="/usr/bin/printf"
declare   CP="/bin/cp"
declare   CHMOD="/bin/chmod"
declare   LS="/bin/ls"


#---(capture the arguments)-------------------#
declare   aVERBOSE="/bin/true"
declare   aBIN_DIR="/home/dotsuu/b_nvdo"
declare   aSCRIPT=""
for ARG in $@; do
   if   [[ "$ARG" == "-v"        ]]; then
      aVERBOSE=""
   elif [[ "$ARG" == "--verbose" ]]; then
      aVERBOSE=""
   elif [[ "$ARG" == "--bin"     ]]; then
      aBIN_DIR="/usr/local/bin"
      $aVERBOSE printf " going to %s\n" "$aBIN_DIR"
   elif [[ "$ARG" == "--sbin"    ]]; then
      aBIN_DIR="/usr/local/sbin"
      $aVERBOSE printf " going to %s\n" "$aBIN_DIR"
   else
      aSCRIPT="$ARG"
   fi
done



$aVERBOSE $PRINTF "#-------------------------------------------------begin---\n"
$aVERBOSE $PRINTF "  program_ : $0\n"
$aVERBOSE $PRINTF "  purpose_ : install scripts and make them executable\n"
$aVERBOSE $PRINTF "#--------------------------------------------------exec---\n"


declare    vType=""


###-------------------------------------------------------------------
###-test-for-program-files--------------------------------------------
###-------------------------------------------------------------------

#---(inform)---------------------------------------------
$aVERBOSE printf " searching for scripts\n"

#---(local/temp variables)-------------------------------
declare    xTypes="cgi py pl sh shh awk"
declare -i xCount=0

#---(process all types)----------------------------------
$aVERBOSE printf "   - base     : $aSCRIPT\n"
$aVERBOSE printf "   - types    :"
for xTest in $xTypes; do
   if [[ -f $aSCRIPT.$xTest ]]; then
      $aVERBOSE printf " ($xTest)"
      vType=$xTest
      ((xCount++))
   else
      $aVERBOSE printf " $xTest"
   fi
done
if [[ -f $aSCRIPT ]]; then
   $aVERBOSE printf " (a.out)"
   vType="a.out"
   ((xCount++))
fi
$aVERBOSE printf "\n"
$aVERBOSE printf "   - found    : $xCount"

#---(test processing results)----------------------------
case "$xCount" in
0)
   $aVERBOSE printf "     [ ERROR , EXITING ]\n"
   exit
   ;;
1)
   $aVERBOSE printf "     [ perfect ]\n"
   ;;
*)
   $aVERBOSE printf "     [ ERROR , EXITING ]\n"
   exit
esac

#---(test logic doesn't fail)----------------------------
if [[ !(-a $aSCRIPT) && !(-a $aSCRIPT.$vType) ]]; then
   $aVERBOSE printf "   - script   : $aSCRIPT.$vType not found, EXITING\n"
   exit
fi




function inst_install {
   ##---(arguments)------------------------------#
   declare a_full="$1"
   declare a_type="$2"
   declare a_name="$3"
   declare a_bdir="$4"
   ##---(move and update)------------------------#
   $aVERBOSE  $PRINTF "   - copying  : %s (%s) to %s\n" "$a_full" "$a_name" "$a_bdir"
   cp  -f "$a_full"  "$a_bdir/$a_name"
   $aVERBOSE  $PRINTF "   - makeexec : %s\n" "$a_name"
   $CHMOD 0755 "$a_bdir/$a_name"
   $aVERBOSE  $PRINTF "   $($LS -loa $a_bdir/$a_name)\n"
   ##---(complete)-------------------------------#
   return 0
}


$aVERBOSE $PRINTF " installing bash script\n"

case $vType in
"cgi")
   $aVERBOSE printf "   - copying  : $aSCRIPT.$vType to ~/l_localhost/cgi-bin/\n"
   cp $aSCRIPT.$vType ~/l_localhost/cgi-bin/$aSCRIPT.$vType
   $aVERBOSE printf "   - makeexec : $aSCRIPT.$vType\n"
   chmod 0755 ~/l_localhost/cgi-bin/$aSCRIPT.$vType
   $aVERBOSE printf "   $(ls -loa ~/l_localhost/cgi-bin/$aSCRIPT.$vType)\n"
   ;;
"shh")
   $aVERBOSE printf "   - copying  : $aSCRIPT.$vType to ~/y_share/\n"
   cp $aSCRIPT.$vType ~/y_share/$aSCRIPT.$vType
   $aVERBOSE printf "   - makeexec : $aSCRIPT.$vType\n"
   chmod 0755 ~/y_share/$aSCRIPT.$vType
   $aVERBOSE printf "   $(ls -loa ~/y_share/$aSCRIPT.$vType)\n"
   ;;
"awk")
   $aVERBOSE printf "   - copying  : $aSCRIPT.$vType to ${aBIN_DIR}/\n"
   cp $aSCRIPT.$vType ${aBIN_DIR}/$aSCRIPT.$vType
   $aVERBOSE printf "   - makeexec : $aSCRIPT.$vType\n"
   chmod 0755 ${aBIN_DIR}/$aSCRIPT.$vType
   $aVERBOSE printf "   $(ls -loa ${aBIN_DIR}/$aSCRIPT.$vType)\n"
   ;;
"a.out")
   inst_install "$aSCRIPT.$vType" "$vType" "$aSCRIPT" "$aBIN_DIR"
   ;;
*)
   inst_install "$aSCRIPT.$vType" "$vType" "$aSCRIPT" "$aBIN_DIR"
   ;;
esac


$aVERBOSE $PRINTF " install complete, script ready\n"
$aVERBOSE $PRINTF "#---------------------------------------------------end---\n"
exit
