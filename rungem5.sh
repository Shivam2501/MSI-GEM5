#!/bin/bash

case "$1" in
	"tagged")
		date=$(date +%d%b%Y.%H%M)
		debugFlags=RubySlicc
		case $2 in
			"blackscholes")
				bname="blackscholes"
				script="./benchmarks/blackscholes.rcS"
				;;

			"bodytrack")
				bname="bodytrack"
				script="./benchmarks/bodytrack.rcS"
				;;

			"fluidanimate")
				bname="fluidanimate"
				script="./benchmarks/fluidanimate.rcS"
				;;

			"x264")
				bname="x264"
				script="./benchmarks/x264.rcS"
				;;
			*)
				echo "Usage:
				$0 build {MI|MSI|MESI|tagged}
				$0 run {blackscholes|bodytrack|fluidanimate|x264}
				$0 runall
				$0 tagged {blackscholes|bodytrack|fluidanimate|x264}"
				exit
				;;
		esac
		# outputDir="./output_logs/${bname}/${date}"
		outputDir="./output_logs/${bname}"
		outName="m5out_${bname}"
		DIR="$( cd "$( dirname "$0" )" && pwd )"
		diskImage="${DIR}/disks/x86root-parsec.img"
		kernelImage="${DIR}/binaries/vmlinux"
		time build/X86/gem5.opt\
			--outdir=${outputDir} --remote-gdb-port=0 configs/example/fs.py\
			--kernel=${kernelImage} --num-cpus=8 --caches --l2cache\
			--disk-image=${diskImage} --script=${script} --standard-switch 4998864102000\
			-I 1000000000

		;;
	"build")
		case $2 in
			"MI")
				protocol="MI_example"
				;;
			"MSI")
				protocol="MSIP"
				;;
			"MESI")
				protocol="MESI_Two_Level"
				;;
			"tagged")
				scons -j8 build/X86/gem5.opt
				exit
				;;
			*)
				echo "Usage:
				$0 build {MI|MSI|MESI|tagged}
				$0 run {blackscholes|bodytrack|fluidanimate|x264}
				$0 runall
				$0 tagged {blackscholes|bodytrack|fluidanimate|x264}"
				exit
				;;
		esac
		scons build/X86/gem5.opt -j8 RUBY=True PROTOCOL=$protocol SLICC_HTML=True
		;;

	"run")
		date=$(date +%d%b%Y.%H%M)
		debugFlags=RubySlicc
		case $2 in
			"blackscholes")
				bname="blackscholes"
				script="./benchmarks/blackscholes.rcS"
				;;

			"bodytrack")
				bname="bodytrack"
				script="./benchmarks/bodytrack.rcS"
				;;

			"fluidanimate")
				bname="fluidanimate"
				script="./benchmarks/fluidanimate.rcS"
				;;

			"oracle")
				bname="oracle"
				script="./benchmarks/oracle.rcS"
				;;

			"x264")
				bname="x264"
				script="./benchmarks/x264.rcS"
				;;
			*)
				echo "Usage:
				$0 build {MI|MSI|MESI|tagged}
				$0 run {blackscholes|bodytrack|fluidanimate|x264}
				$0 runall
				$0 tagged {blackscholes|bodytrack|fluidanimate|x264}"
				exit
				;;
		esac
		# outputDir="./output_logs/${bname}/${date}"
		outputDir="./output_logs/${bname}"
		outName="m5out_${bname}"
		DIR="$( cd "$( dirname "$0" )" && pwd )"
		diskImage="${DIR}/disks/x86root-parsec.img"
		kernelImage="${DIR}/binaries/vmlinux"

		time build/X86/gem5.opt\
			--outdir=${outputDir} --remote-gdb-port=0 configs/example/fs.py\
			--kernel=${kernelImage} --ruby --num-cpus=8 --caches --l2cache\
			--disk-image=${diskImage} --script=${script} --cpu-type=TimingSimpleCPU\
			--num-dirs=8 --num-l2caches=8 --topology=Crossbar -I 300000000
		;;
	runall)
		for bname in "blackscholes" "bodytrack" "fluidanimate" "x264"
		do
			date=$(date +%d%b%Y.%H%M)
			debugFlags=RubySlicc
			case $bname in
				"blackscholes")
					script="./blackscholes.rcS"
					;;

				"bodytrack")
					script="./bodytrack.rcS"
					;;

				"fluidanimate")
					script="./fluidanimate.rcS"
					;;

				"x264")
					script="./x264.rcS"
					;;
			esac
			outputDir="./output_logs/${bname}"
			outName="m5out_${bname}"
			DIR="$( cd "$( dirname "$0" )" && pwd )"
			diskImage="${DIR}/disks/x86root-parsec.img"
			kernelImage="${DIR}/binaries/vmlinux"

			time build/X86/gem5.opt\
				--outdir=${outputDir} --remote-gdb-port=0 configs/example/fs.py\
				--kernel=${kernelImage} --ruby --num-cpus=8 --caches --l2cache\
				--disk-image=${diskImage} --script=${script} --cpu-type=TimingSimpleCPU\
				--num-dirs=8 --num-l2caches=8 --topology=Crossbar -I 300000000
		done
		;;
	*)
		echo "Usage:
		$0 build {MI|MSI|MESI|tagged}
		$0 run {blackscholes|bodytrack|fluidanimate|x264}
		$0 runall
		$0 tagged {blackscholes|bodytrack|fluidanimate|x264}"
		;;
esac
