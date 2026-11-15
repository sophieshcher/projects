# CERN Open Data `.root` Files Directory

Place your downloaded `.root` file in this directory (for example, `data/Dimuon_DoubleMu.root`). All `*.root` files are ignored by `.gitignore` to keep the Git repository lightweight.

## Direct Download Commands

### Option 1: Lightweight CMS $Z \to \mu^+\mu^-$ Ntuple (`0.17 MB`)
```bash
curl -L https://raw.githubusercontent.com/scikit-hep/scikit-hep-testdata/main/src/skhep_testdata/data/uproot-Zmumu.root -o data/Dimuon_DoubleMu.root
```
- **Tree name:** `events`
- **Branches:** `pt1, eta1, phi1, Q1, E1, px1, py1, pz1, pt2, eta2, phi2, Q2, E2, px2, py2, pz2, M`

### Option 2: Full CMS 2012 `DoubleMuParked` NanoAOD ([CERN Open Data Record #12341](https://opendata.cern.ch/record/12341), `2.14 GB`)
```bash
curl -L https://opendata.cern.ch/eos/opendata/cms/derived-data/AOD2NanoAODOutreachTool/Run2012BC_DoubleMuParked_Muons.root -o data/Run2012BC_DoubleMuParked_Muons.root
```
- **Tree name:** `Events`
- **Branches:** `nMuon, Muon_pt[nMuon], Muon_eta[nMuon], Muon_phi[nMuon], Muon_mass[nMuon], Muon_charge[nMuon]`
