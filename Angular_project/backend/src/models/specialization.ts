import mongoose from "mongoose";

const Schema = mongoose.Schema;

let Specialization = new Schema({
    specialization: {
        type: String
    }
})

export default mongoose.model('SpecializationModel', Specialization, 'specializations')