#!/bin/csh -f

foreach x (`seq 0 4`)
foreach QQ (`seq 0 1`)
foreach z (`seq 0 17`)
foreach PT2 (`seq 0 19`)

if(!($x == 4 && $QQ == 1)) then

	root -l -b -q "systematics_v2.C($x,$QQ,$z,$PT2,"'"pip")'
	root -l -b -q "systematics_v2.C($x,$QQ,$z,$PT2,"'"pim")'

	root -l -b -q "sector_systematics.C($x,$QQ,$z,$PT2,"'"pip")'
	root -l -b -q "sector_systematics.C($x,$QQ,$z,$PT2,"'"pim")'

	root -l -b -q "total_systematics.C($x,$QQ,$z,$PT2,"'"pip")'
	root -l -b -q "total_systematics.C($x,$QQ,$z,$PT2,"'"pim")'

endif

end
end
end
end
