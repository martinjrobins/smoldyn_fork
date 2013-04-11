# Regression testing for Smoldyn
import os
import filecmp
import sys

filelist=['addspecies','allostery','bounce2','bounds1','bounds2',
					'cmdmanipulate','compartlogic','compartrxn','crowding',
					'diffa','drift','dumbbell',
					'emitter1','lotvolt',
					'molecule','rafts',
					'surf1','surf2','surf3','surfaceboundjump','stick']

suffix=raw_input('Enter suffix for new simulations: ')
reference=raw_input('Enter suffix for reference simulations or "none": ')

if reference!='none':
	dirname='output_%s/' %(reference)
	if not os.path.exists(dirname):
		print('reference directory "%s" does not exist' %(reference))
		sys.exit()

dirname='output_%s' %(suffix)
try:
	os.makedirs(dirname)
except OSError:
	if os.path.exists(dirname):
		pass
	else:
		raise

for filename in filelist:
	filenew='output_'+suffix+'/'+filename+'_'+suffix+'_out.txt'
	fileref='output_'+reference+'/'+filename+'_'+reference+'_out.txt'
	string='smoldyn %s.txt --define OUTFILE=%s -tqw' %(filename,filenew)
	os.system(string)
	if reference=='none':
		print('finished %s.txt' %(filename))
	elif not os.path.exists(fileref):
		print('added %s.txt' %(filename))
	else:
		same=filecmp.cmp(filenew,fileref)
		if same:
			print('same: %s.txt' %(filename))
		else:
			print('DIFFERENT: %s.txt' %(filename))
