
#include "mol/mzrModMol.hh"
#include "mol/mzrMol.hh"
#include "nmrUnit.hh"
#include "mol/molUnit.hh"
#include "complexFormNameAssembler.hh"
#include "complexOutputState.hh"

#include <string>
#include <vector>


using std::string;
using std::vector;

namespace nmr
{

    string complexFormNameAssembler::createNameFromOutputState( ComplexOutputStateCref aCOS) const
    {
        vector<string> molecule_representations;
        for (unsigned int mol_ndx = 0; mol_ndx != aCOS.theMolTokens.size(); ++mol_ndx)
        {
            string mol_name(aCOS.theMolTokens[mol_ndx]);

            // Here, we iterate through the ComplexOutputState to
            // collect all binding and modifications that belong to the molecule in question.

            bool molecule_is_simple = this->isMoleculeSimple( aCOS, mol_ndx );

            vector<int> bnds_attached_to_mol;
            vector<int> mods_attached_to_mol;

            this->getMoleculeComponents(aCOS, mol_ndx, bnds_attached_to_mol, mods_attached_to_mol);

            // Now use the information we have (current molecule ndx
            // in the COS, the associated bindings, the associated
            // modifications ) to produce the name.

            if (molecule_is_simple)
            {
                // TODO - RAISE EXCEPTION
                if (bnds_attached_to_mol.size() > 1 || mods_attached_to_mol.size() != 0){}
                if (bnds_attached_to_mol.size() == 0)
                {
                    mol_name += "()";
                }
                else
                {
                    mol_name += "(!" + utl::stringify( bnds_attached_to_mol[0] ) + ")";
                }

                molecule_representations.push_back( mol_name );
            }
            else
            {
                // if (bnds_attached_to_mol.size() == 0) {} // TODO - RAISE EXCEPTION

                // We are going to assemble a list of components into the binding site.
                // These will be something like "site_1!1", "site_2!10", "mod~value", ...
                vector<string> intra_molecule_components;
                string mol_ndx_stringified = utl::stringify(mol_ndx);

                // Go through the bindings attached to this mol.  Figure out what the name of that binding site is
                for( unsigned int ndx = 0; ndx != bnds_attached_to_mol.size(); ++ndx)
                {
                    int binding_ndx = bnds_attached_to_mol[ndx];
                    string binding_ndx_stringified = utl::stringify( binding_ndx );


                    const ComplexOutputState::BindingTokenStr& current_binding( aCOS.theBindingTokens[binding_ndx] );
                    string binding_site_ndx_stringified("");
                    if (current_binding.first.first == mol_ndx_stringified) binding_site_ndx_stringified = current_binding.first.second;
                    else binding_site_ndx_stringified = current_binding.second.second;

                    // After this code, mol_binding_site_ndx should
                    // have within it the index of the binding site in
                    // the molecule for appropriate binding.  
                    int mol_binding_site_ndx;
                    utl::stringIsInt( binding_site_ndx_stringified, mol_binding_site_ndx);

                    const bnd::molUnit& theMolUnit( *theNmrUnit->pMolUnit );
                    const utl::autoCatalog<bnd::mzrMol>& molsByName = theMolUnit.getMolsByName();
                    const bnd::mzrMol* pMzrMol = molsByName.findEntry(mol_name);
                        
                    string binding_site_name( (*pMzrMol)[mol_binding_site_ndx].getName());
                    string bnd_component = (binding_site_name + "!" + binding_ndx_stringified);

                    intra_molecule_components.push_back(bnd_component);
                }


                // Now go through each of the modifications and add
                // it's component to the list of intra molecule
                // components.
                for ( unsigned int modification_ndx = 0; modification_ndx != mods_attached_to_mol.size(); ++modification_ndx)
                {
                    int mod_num = mods_attached_to_mol[modification_ndx];
                    const ComplexOutputState::ModificationTokenStr& theMod(aCOS.theModificationTokens[mod_num]);

                    string modification_name( theMod.second.first );
                    string modification_value( theMod.second.second );

                    string mod_component = "*" + modification_name + "{" + modification_value + "}";
                    intra_molecule_components.push_back(mod_component);
                }
               
                // Combine the intramolecular components into a mol_name
                string internal_representation("");
                
                for(unsigned int comp_ndx = 0; comp_ndx != intra_molecule_components.size(); ++comp_ndx)
                {
                    internal_representation += ( intra_molecule_components[comp_ndx] + ",");
                }

                if (intra_molecule_components.size() > 0)
                {
                    internal_representation = internal_representation.substr( 0 , internal_representation.length() - 1 );
                }

                mol_name += ( "(" + internal_representation + ")" );
                molecule_representations.push_back( mol_name );
            }

        }

        string molecule_name("");
        for(unsigned int mol_repr_ndx = 0; mol_repr_ndx != molecule_representations.size(); ++mol_repr_ndx)
        {
            molecule_name += molecule_representations[mol_repr_ndx];
            molecule_name += ".";
        }

        molecule_name = molecule_name.substr(0, molecule_name.size()-1);
        return molecule_name;
}

    bool complexFormNameAssembler::getMoleculeComponents( ComplexOutputStateCref aCOS, unsigned int mol_ndx, \
                                                          std::vector<int>& bnds_attached_to_mol, \
                                                          std::vector<int>& mods_attached_to_mol) const
    {
        string mol_ndx_stringified = utl::stringify( mol_ndx );
        
        for( unsigned int bnd_ndx = 0; bnd_ndx != aCOS.theBindingTokens.size(); ++bnd_ndx)
        {
            const ComplexOutputState::BindingTokenStr& currentBinding = aCOS.theBindingTokens[bnd_ndx];
            if (currentBinding.first.first == mol_ndx_stringified || currentBinding.second.first == mol_ndx_stringified)
            {
                bnds_attached_to_mol.push_back( bnd_ndx );
            }
        }

        for( unsigned int mod_ndx = 0; mod_ndx != aCOS.theModificationTokens.size(); ++mod_ndx)
        {
            const ComplexOutputState::ModificationTokenStr& currentModification = aCOS.theModificationTokens[mod_ndx];
            if (currentModification.first == mol_ndx_stringified) 
            {
                mods_attached_to_mol.push_back(mod_ndx);
            }
        }

        return true;
    }


    bool complexFormNameAssembler::isMoleculeSimple( ComplexOutputStateCref aCOS, unsigned int mol_ndx) const
    {
        const string& molecule_name(aCOS.theMolTokens[mol_ndx]);
        
        const bnd::molUnit& theMolUnit( *theNmrUnit->pMolUnit );
        const utl::autoCatalog<bnd::mzrMol>& molsByName = theMolUnit.getMolsByName();

        const bnd::mzrMol* pMzrMol = molsByName.findEntry(molecule_name);

        const bnd::mzrModMol* pModMol = 0;
        pModMol = dynamic_cast<const bnd::mzrModMol*>( pMzrMol );

        return (pModMol == 0);
    }

}
