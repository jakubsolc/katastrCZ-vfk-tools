%%%%%%%%%%%%%%  E X P O R T    A L L  %%%%%%%%%%%%%%%%%%%%%%
close all, clear all
fsize = 10;
lw_pc = 0.8;
lw_bu = 0.2;
no_bd = 16;
no_bu = 4;
no_co = 0;

%% No of boundary: 16
%% No of building: 4
%% No of coordset: 0

%%%% Budova id=824950209 na na parcele "6/1" :1

BU{1} = [
    1228  728303.38   1036271.50
     253  728308.49   1036274.06
     252  728307.05   1036278.86
    1230  728301.17   1036275.92
     254  728300.40   1036277.46
     255  728289.79   1036272.15
     256  728283.77   1036292.51
     202  728274.14   1036289.94
    1247  728277.25   1036266.18
     263  728280.99   1036267.09
     262  728285.55   1036269.15
     261  728288.14   1036263.85
    1228  728303.38   1036271.50
];
%%--------------------------------------------------------
%%%% Budova id=825057209 na na parcele "7/1" :1

BU{2} = [
     358  728332.26   1036252.65
    1212  728324.38   1036249.34
    1219  728314.94   1036271.80
     361  728322.82   1036275.11
     358  728332.26   1036252.65
];
%%--------------------------------------------------------
%%%% Budova id=825056209 na na parcele "5" :1

BU{3} = [
    1246  728279.44   1036239.24
     205  728269.21   1036235.87
    1252  728265.40   1036247.43
    1249  728275.63   1036250.81
    1246  728279.44   1036239.24
];
%%--------------------------------------------------------
%%%% Budova id=825035209 na na parcele "11" :1

BU{4} = [
    1186  728340.40   1036264.71
    1181  728344.39   1036266.75
    1182  728343.73   1036268.05
    1170  728353.47   1036273.02
    1168  728357.83   1036264.50
    1176  728348.09   1036259.52
    1177  728347.06   1036261.53
    1184  728343.06   1036259.49
    1186  728340.40   1036264.71
];
%%--------------------------------------------------------
%%%% Hranice parcely  "6/1" (st.)      op_id=3094436209

BD{1} = [
     251  728327.07   1036280.33
     250  728322.04   1036277.23
     253  728308.49   1036274.06
    1228  728303.38   1036271.50
     261  728288.14   1036263.85
     262  728285.55   1036269.15
     263  728280.99   1036267.09
    1247  728277.25   1036266.18
     202  728274.14   1036289.94
     256  728283.77   1036292.51
     249  728314.46   1036302.35
     251  728327.07   1036280.33
];
%%--------------------------------------------------------
%%%% Hranice parcely  "284" (poz.)     op_id=29603396010

BD{2} = [
       4  728273.22   1036293.30
       1  728312.43   1036305.96
     249  728314.46   1036302.35
     256  728283.77   1036292.51
     202  728274.14   1036289.94
       4  728273.22   1036293.30
];
%%--------------------------------------------------------
%%%% Hranice parcely  "13/1" (poz.)     op_id=3094658209

BD{3} = [
     263  728280.99   1036267.09
     262  728285.55   1036269.15
     261  728288.14   1036263.85
       3  728266.68   1036255.25
     225  728263.27   1036253.89
      42  728254.51   1036283.72
      75  728253.59   1036286.84
       2  728261.78   1036289.61
       4  728273.22   1036293.30
     202  728274.14   1036289.94
    1247  728277.25   1036266.18
     263  728280.99   1036267.09
];
%%--------------------------------------------------------
%%%% Hranice parcely  "7/1" (st.)      op_id=3094438209

BD{4} = [
    1229  728302.34   1036255.71
    1228  728303.38   1036271.50
     253  728308.49   1036274.06
     250  728322.04   1036277.23
     361  728322.82   1036275.11
     358  728332.26   1036252.65
    1212  728324.38   1036249.34
    1222  728307.92   1036242.42
    1224  728306.44   1036245.94
    1232  728298.44   1036242.59
    1234  728294.33   1036252.35
    1229  728302.34   1036255.71
];
%%--------------------------------------------------------
%%%% Hranice parcely  "5" (st.)      op_id=3094435209

BD{5} = [
     211  728270.64   1036233.73
     205  728269.21   1036235.87
    1252  728265.40   1036247.43
     225  728263.27   1036253.89
       3  728266.68   1036255.25
     261  728288.14   1036263.85
    1228  728303.38   1036271.50
    1229  728302.34   1036255.71
    1234  728294.33   1036252.35
    1232  728298.44   1036242.59
    1224  728306.44   1036245.94
    1222  728307.92   1036242.42
    1221  728310.03   1036237.40
    1227  728303.76   1036235.35
    1236  728292.98   1036234.57
     211  728270.64   1036233.73
];
%%--------------------------------------------------------
%%%% Hranice parcely  "2/1" (poz.)     op_id=55741459010

BD{6} = [
    1238  728292.31   1036346.36
      28  728262.80   1036332.99
      33  728265.56   1036326.16
      34  728273.39   1036306.47
      32  728274.53   1036303.60
       2  728285.75   1036307.23
       1  728308.27   1036314.22
      41  728306.92   1036316.97
    1238  728292.31   1036346.36
];
%%--------------------------------------------------------
%%%% Hranice parcely  "2/2" (poz.)     op_id=55741460010

BD{7} = [
      32  728274.53   1036303.60
      34  728273.39   1036306.47
      33  728265.56   1036326.16
      30  728246.90   1036317.95
      35  728253.93   1036300.28
      31  728255.18   1036297.13
       9  728256.17   1036297.53
       3  728265.64   1036300.73
      32  728274.53   1036303.60
];
%%--------------------------------------------------------
%%%% Hranice parcely  "2/3" (poz.)     op_id=55741461010

BD{8} = [
      28  728262.80   1036332.99
      29  728244.23   1036324.57
      30  728246.90   1036317.95
      33  728265.56   1036326.16
      28  728262.80   1036332.99
];
%%--------------------------------------------------------
%%%% Hranice parcely  "2/4" (poz.)     op_id=55741458010

BD{9} = [
      29  728244.23   1036324.57
    1259  728237.43   1036321.49
     131  728240.07   1036315.01
      30  728246.90   1036317.95
      29  728244.23   1036324.57
];
%%--------------------------------------------------------
%%%% Hranice parcely  "23/2" (poz.)     op_id=3094668209

BD{10} = [
    1252  728265.40   1036247.43
     205  728269.21   1036235.87
     130  728251.41   1036230.43
      76  728233.74   1036276.96
      89  728242.78   1036281.88
      75  728253.59   1036286.84
      42  728254.51   1036283.72
     225  728263.27   1036253.89
    1252  728265.40   1036247.43
];
%%--------------------------------------------------------
%%%% Hranice parcely  "23/3" (poz.)     op_id=51484094010

BD{11} = [
      76  728233.74   1036276.96
      77  728229.74   1036274.53
      90  728221.96   1036269.31
      78  728216.36   1036264.42
     129  728236.43   1036216.67
     207  728236.92   1036219.88
     206  728249.75   1036229.92
     130  728251.41   1036230.43
      76  728233.74   1036276.96
];
%%--------------------------------------------------------
%%%% Hranice parcely  "23/6" (poz.)     op_id=51484097010

BD{12} = [
       6  728231.02   1036311.15
       7  728200.12   1036297.60
      14  728213.87   1036274.33
      51  728215.16   1036275.40
      52  728220.00   1036279.03
      13  728225.11   1036282.25
      12  728229.10   1036284.67
      11  728237.87   1036289.48
       4  728241.83   1036291.44
      10  728246.90   1036293.77
      31  728255.18   1036297.13
      35  728253.93   1036300.28
      30  728246.90   1036317.95
     131  728240.07   1036315.01
       6  728231.02   1036311.15
];
%%--------------------------------------------------------
%%%% Hranice parcely  "178/4" (poz.)     op_id=57723612010

BD{13} = [
      11  728312.46   1036315.63
       1  728312.43   1036305.96
      10  728316.85   1036307.46
      40  728311.70   1036307.25
       1  728308.27   1036314.22
      11  728312.46   1036315.63
];
%%--------------------------------------------------------
%%%% Hranice parcely  "11" (st.)      op_id=3094442209

BD{14} = [
     349  728373.35   1036255.88
      19  728363.79   1036250.97
    1179  728344.66   1036253.01
    1191  728338.31   1036263.25
      21  728336.97   1036269.90
    1190  728339.23   1036274.65
    1180  728344.42   1036277.46
       4  728358.32   1036276.20
     350  728363.07   1036269.77
     349  728373.35   1036255.88
];
%%--------------------------------------------------------
%%%% Hranice parcely  "4/11" (poz.)     op_id=19529671010

BD{15} = [
       5  728365.50   1036315.67
      19  728377.37   1036286.98
      18  728331.59   1036280.03
      10  728316.85   1036307.46
       5  728365.50   1036315.67
];
%%--------------------------------------------------------
%%%% Hranice parcely  "4/10" (poz.)     op_id=3924931209

BD{16} = [
      19  728377.37   1036286.98
      23  728388.15   1036263.48
     347  728386.38   1036262.57
     349  728373.35   1036255.88
     350  728363.07   1036269.77
       4  728358.32   1036276.20
    1180  728344.42   1036277.46
    1190  728339.23   1036274.65
      21  728336.97   1036269.90
      18  728331.59   1036280.03
      19  728377.37   1036286.98
];
%%--------------------------------------------------------
figure;
hold on
set(gca,'box','on')
for k = 1:no_bu
   fill( -BU{k}(:,2), -BU{k}(:,3), 'y', 'linewidth', lw_bu);
   plot( -BU{k}(:,2), -BU{k}(:,3), 'linewidth', lw_bu);
end;
for k = 1:no_bd
   plot( -BD{k}(:,2), -BD{k}(:,3), 'linewidth', lw_pc);
end
daspect([1 1 1]);
for k = 1:no_bu
   for bod = 1 : (size(BU{k},1)-1)  %% last=first
      text(-BU{k}(bod,2), -BU{k}(bod,3), sprintf('%3d', BU{k}(bod,1)), 'color', 'b', 'fontsize',fsize);
   end
end
for k = 1:no_bd
   for bod = 1 : (size(BD{k},1)-1)  %% last=first
      text(-BD{k}(bod,2), -BD{k}(bod,3), sprintf('%3d', BD{k}(bod,1)), 'fontsize',fsize);
   end
end

%%---------------------------------------------------
yticks = get (gca, 'ytick');
ylabels = arrayfun (@(x) sprintf ('%.0f', x), yticks, 'uniformoutput', false);
set (gca, 'yticklabel', ylabels)

%% print -dpdfwrite -r300 -landscape -F:5 t3.pdf
set(gcf,'paperorientation','landscape')
set(gca,'ylim',[-1036350 -1036200])

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%print -dpdfwrite -r300 -landscape -F:5 tX.pdf